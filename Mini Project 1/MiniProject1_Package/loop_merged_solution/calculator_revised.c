#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/*============================================================================================
  Definitions
============================================================================================*/
#define MAXLEN 256
#define TBLSIZE 256
#define REGCOUNT 8   /* r0..r7; r0,r1,r2 reserved for x,y,z at EXIT */
#define PRINTERR 0

/* ---- Token types ---- */
typedef enum {
    UNKNOWN, END, ENDFILE,
    INT, ID,
    ADDSUB,       /* + or - (single) */
    MULDIV,       /* * or / */
    ASSIGN,       /* = */
    ADDSUB_ASSIGN,/* += or -= */
    INCDEC,       /* ++ or -- */
    LPAREN, RPAREN,
    AND,          /* & */
    OR,           /* | */
    XOR           /* ^ */
} TokenSet;

/* ---- Error types ---- */
typedef enum {
    UNDEFINED, MISPAREN, NOTNUMID, NOTFOUND, RUNOUT,
    NOTLVAL, DIVZERO, SYNTAXERR
} ErrorType;

/* ---- Symbol table entry ---- */
typedef struct {
    char name[MAXLEN];
    int  memAddr;   /* memory address (-1 = not in memory yet) */
    int  reg;       /* register holding current value (-1 = none) */
    int  defined;   /* 1 = has been assigned at least once (or is x/y/z) */
} Symbol;

/* ---- AST node ---- */
typedef struct _Node {
    TokenSet    data;
    int         val;
    char        lexeme[MAXLEN];
    struct _Node *left;
    struct _Node *right;
} BTNode;

/*============================================================================================
  Globals
============================================================================================*/
/* Lexer */
static char    lexeme[MAXLEN];
static TokenSet curToken = UNKNOWN;

/* Symbol table */
static int    sbcount = 0;
static Symbol table[TBLSIZE];

/* Next available memory address for new variables (x=0,y=4,z=8, new starts at 12) */
static int    nextMem = 12;

/* Register allocator: simple "next free register" (never spill in this impl) */
/* r0,r1,r2 are the "home" of x,y,z but we use temps freely;
   we just pick the lowest free register each time. */
static int    regUsed[REGCOUNT];   /* 0 = free */

/* Output buffer so we can prepend MOV r0,[0] etc. at the very top once */
#define OUTBUF 65536
static char outBuf[OUTBUF];
static int  outPos = 0;

/* Total clock cycles */
static long long totalCycles = 0;

/* Flag: error encountered */
static int hasError = 0;

/*============================================================================================
  Output helpers
============================================================================================*/
static void emit(const char *fmt, ...) {
    va_list ap;
    char tmp[512];
    va_start(ap, fmt);
    vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    int len = strlen(tmp);
    if (outPos + len < OUTBUF - 1) {
        memcpy(outBuf + outPos, tmp, len);
        outPos += len;
        outBuf[outPos] = '\0';
    }
}

static void emitMOV_RR(int dst, int src) {
    emit("MOV r%d, r%d\n", dst, src);
    totalCycles += 10;
}
static void emitMOV_RC(int dst, int c) {
    emit("MOV r%d, %d\n", dst, c);
    totalCycles += 10;
}
static void emitMOV_RM(int dst, int addr) {
    emit("MOV r%d, [%d]\n", dst, addr);
    totalCycles += 200;
}
static void emitMOV_MR(int addr, int src) {
    emit("MOV [%d], r%d\n", addr, src);
    totalCycles += 200;
}
static void emitADD(int dst, int src) { emit("ADD r%d, r%d\n", dst, src); totalCycles += 10; }
static void emitSUB(int dst, int src) { emit("SUB r%d, r%d\n", dst, src); totalCycles += 10; }
static void emitMUL(int dst, int src) { emit("MUL r%d, r%d\n", dst, src); totalCycles += 30; }
static void emitDIV(int dst, int src) { emit("DIV r%d, r%d\n", dst, src); totalCycles += 50; }
static void emitAND(int dst, int src) { emit("AND r%d, r%d\n", dst, src); totalCycles += 10; }
static void emitOR (int dst, int src) { emit("OR r%d, r%d\n",  dst, src); totalCycles += 10; }
static void emitXOR(int dst, int src) { emit("XOR r%d, r%d\n", dst, src); totalCycles += 10; }

/*============================================================================================
  Register allocator
============================================================================================*/
static int allocReg(void) {
    for (int i = 0; i < REGCOUNT; i++) {
        if (!regUsed[i]) {
            regUsed[i] = 1;
            return i;
        }
    }
    /* Ran out — shouldn't happen for reasonable inputs */
    fprintf(stderr, "FATAL: out of registers\n");
    exit(1);
}
static void freeReg(int r) {
    if (r >= 0 && r < REGCOUNT)
        regUsed[r] = 0;
}
/* "Borrow" a register without marking it used */
/* Not needed — allocReg/freeReg suffices */

/*============================================================================================
  Symbol table
============================================================================================*/
static void initTable(void) {
    /* x at [0], y at [4], z at [8] */
    strcpy(table[0].name, "x"); table[0].memAddr = 0;  table[0].reg = -1; table[0].defined = 1;
    strcpy(table[1].name, "y"); table[1].memAddr = 4;  table[1].reg = -1; table[1].defined = 1;
    strcpy(table[2].name, "z"); table[2].memAddr = 8;  table[2].reg = -1; table[2].defined = 1;
    sbcount = 3;
    nextMem = 12;
}

/* Return index in table, or -1 if not found */
static int findSym(const char *name) {
    for (int i = 0; i < sbcount; i++)
        if (strcmp(table[i].name, name) == 0)
            return i;
    return -1;
}

/* Add a new variable; assign memory address */
static int addSym(const char *name) {
    if (sbcount >= TBLSIZE) { hasError = 1; return -1; }
    strcpy(table[sbcount].name, name);
    table[sbcount].memAddr = nextMem;
    table[sbcount].reg     = -1;
    table[sbcount].defined = 0;
    nextMem += 4;
    return sbcount++;
}

/*============================================================================================
  Lexer
============================================================================================*/
static TokenSet getToken(void) {
    int i = 0;
    char c;
    while ((c = fgetc(stdin)) == ' ' || c == '\t');

    if (c == EOF) { lexeme[0] = '\0'; return ENDFILE; }
    if (c == '\n') { lexeme[0] = '\0'; return END; }

    if (isdigit(c)) {
        lexeme[0] = c; i = 1;
        while (isdigit(c = fgetc(stdin)) && i < MAXLEN-1) lexeme[i++] = c;
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    }

    /* ID: starts with alpha or _ */
    if (isalpha(c) || c == '_') {
        lexeme[0] = c; i = 1;
        while ((isalnum(c = fgetc(stdin)) || c == '_') && i < MAXLEN-1) lexeme[i++] = c;
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    }

    if (c == '+' || c == '-') {
        char next = fgetc(stdin);
        if (next == c) {
            /* ++ or -- */
            lexeme[0] = c; lexeme[1] = c; lexeme[2] = '\0';
            return INCDEC;
        } else if (next == '=') {
            lexeme[0] = c; lexeme[1] = '='; lexeme[2] = '\0';
            return ADDSUB_ASSIGN;
        } else {
            ungetc(next, stdin);
            lexeme[0] = c; lexeme[1] = '\0';
            return ADDSUB;
        }
    }

    if (c == '*' || c == '/') { lexeme[0]=c; lexeme[1]='\0'; return MULDIV; }

    if (c == '=') { strcpy(lexeme, "=");  return ASSIGN; }
    if (c == '(') { strcpy(lexeme, "(");  return LPAREN; }
    if (c == ')') { strcpy(lexeme, ")");  return RPAREN; }
    if (c == '&') { strcpy(lexeme, "&");  return AND; }
    if (c == '|') { strcpy(lexeme, "|");  return OR; }
    if (c == '^') { strcpy(lexeme, "^");  return XOR; }

    lexeme[0] = c; lexeme[1] = '\0';
    return UNKNOWN;
}

static void advance(void)    { curToken = getToken(); }
static int  match(TokenSet t){ if (curToken == UNKNOWN) advance(); return t == curToken; }
static char *getLexeme(void) { return lexeme; }

/*============================================================================================
  Error handling
============================================================================================*/
static void err(ErrorType e) {
    if (PRINTERR) {
        fprintf(stderr, "error: ");
        switch(e){
            case MISPAREN:  fprintf(stderr,"mismatched parenthesis\n"); break;
            case NOTNUMID:  fprintf(stderr,"number or identifier expected\n"); break;
            case NOTFOUND:  fprintf(stderr,"variable not defined\n"); break;
            case RUNOUT:    fprintf(stderr,"out of memory\n"); break;
            case NOTLVAL:   fprintf(stderr,"lvalue required as an operand\n"); break;
            case DIVZERO:   fprintf(stderr,"divide by constant zero\n"); break;
            case SYNTAXERR: fprintf(stderr,"syntax error\n"); break;
            default:        fprintf(stderr,"undefined error\n"); break;
        }
    }
    /* drain rest of line */
    {
        int ch;
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
    }
    hasError = 1;
}

/*============================================================================================
  AST helpers
============================================================================================*/
static BTNode *makeNode(TokenSet tok, const char *lex) {
    BTNode *n = (BTNode*)malloc(sizeof(BTNode));
    n->data  = tok;
    n->val   = 0;
    strcpy(n->lexeme, lex);
    n->left  = NULL;
    n->right = NULL;
    return n;
}
static void freeTree(BTNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/*============================================================================================
  Parser  (implements the full grammar from the spec)

  statement    := ENDFILE | END | assign_expr END
  assign_expr  := ID ASSIGN assign_expr | ID ADDSUB_ASSIGN assign_expr | or_expr
  or_expr      := xor_expr or_expr_tail
  or_expr_tail := OR xor_expr or_expr_tail | NiL
  xor_expr     := and_expr xor_expr_tail
  xor_expr_tail:= XOR and_expr xor_expr_tail | NiL
  and_expr     := addsub_expr and_expr_tail
  and_expr_tail:= AND addsub_expr and_expr_tail | NiL
  addsub_expr  := muldiv_expr addsub_expr_tail
  addsub_expr_tail := ADDSUB muldiv_expr addsub_expr_tail | NiL
  muldiv_expr  := unary_expr muldiv_expr_tail
  muldiv_expr_tail := MULDIV unary_expr muldiv_expr_tail | NiL
  unary_expr   := ADDSUB unary_expr | factor
  factor       := INT | ID | INCDEC ID | LPAREN assign_expr RPAREN
============================================================================================*/

/* Forward declarations */
static BTNode *assign_expr(void);
static BTNode *or_expr(void);
static BTNode *xor_expr(void);
static BTNode *and_expr(void);
static BTNode *addsub_expr(void);
static BTNode *muldiv_expr(void);
static BTNode *unary_expr(void);
static BTNode *factor(void);

static BTNode *factor(void) {
    BTNode *retp = NULL;
    if (hasError) return makeNode(INT,"0");

    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
    } else if (match(ID)) {
        retp = makeNode(ID, getLexeme());
        advance();
    } else if (match(INCDEC)) {
        /* INCDEC ID */
        char op[MAXLEN];
        strcpy(op, getLexeme());
        advance();
        if (!match(ID)) { err(NOTNUMID); return makeNode(INT,"0"); }
        retp = makeNode(INCDEC, op);
        retp->left = makeNode(ID, getLexeme());
        advance();
    } else if (match(LPAREN)) {
        advance();
        retp = assign_expr();
        if (match(RPAREN)) advance();
        else err(MISPAREN);
    } else {
        err(NOTNUMID);
        return makeNode(INT,"0");
    }
    return retp;
}

static BTNode *unary_expr(void) {
    if (hasError) return makeNode(INT,"0");
    if (match(ADDSUB)) {
        char op[MAXLEN];
        strcpy(op, getLexeme());
        BTNode *node = makeNode(ADDSUB, op);
        node->left = makeNode(INT, "0");
        advance();
        node->right = unary_expr();
        return node;
    }
    return factor();
}

static BTNode *muldiv_expr(void) {
    if (hasError) return makeNode(INT,"0");
    BTNode *left = unary_expr();
    while (!hasError && match(MULDIV)) {
        BTNode *node = makeNode(MULDIV, getLexeme());
        advance();
        node->left  = left;
        node->right = unary_expr();
        left = node;
    }
    return left;
}

static BTNode *addsub_expr(void) {
    if (hasError) return makeNode(INT,"0");
    BTNode *left = muldiv_expr();
    while (!hasError && match(ADDSUB)) {
        BTNode *node = makeNode(ADDSUB, getLexeme());
        advance();
        node->left  = left;
        node->right = muldiv_expr();
        left = node;
    }
    return left;
}

static BTNode *and_expr(void) {
    if (hasError) return makeNode(INT,"0");
    BTNode *left = addsub_expr();
    while (!hasError && match(AND)) {
        BTNode *node = makeNode(AND, "&");
        advance();
        node->left  = left;
        node->right = addsub_expr();
        left = node;
    }
    return left;
}

static BTNode *xor_expr(void) {
    if (hasError) return makeNode(INT,"0");
    BTNode *left = and_expr();
    while (!hasError && match(XOR)) {
        BTNode *node = makeNode(XOR, "^");
        advance();
        node->left  = left;
        node->right = and_expr();
        left = node;
    }
    return left;
}

static BTNode *or_expr(void) {
    if (hasError) return makeNode(INT,"0");
    BTNode *left = xor_expr();
    while (!hasError && match(OR)) {
        BTNode *node = makeNode(OR, "|");
        advance();
        node->left  = left;
        node->right = xor_expr();
        left = node;
    }
    return left;
}

/*
  assign_expr := ID ASSIGN assign_expr
               | ID ADDSUB_ASSIGN assign_expr
               | or_expr

  We need look-ahead: if current token is ID and next is ASSIGN/ADDSUB_ASSIGN
  we take the assignment branch; otherwise fall through to or_expr.

  Because our lexer is one-token, we peek by remembering that match(ID) is true
  and we already have the lexeme.  We call advance() to get the next token and
  check it.
*/
static BTNode *assign_expr(void) {
    if (hasError) return makeNode(INT,"0");

    /* Peek: if ID followed by ASSIGN or ADDSUB_ASSIGN */
    if (match(ID)) {
        char savedLex[MAXLEN];
        strcpy(savedLex, getLexeme());
        /* We have the ID. Get the next token. */
        advance(); /* now curToken is whatever follows */

        if (match(ASSIGN)) {
            /* ID ASSIGN assign_expr */
            BTNode *lhs = makeNode(ID, savedLex);
            BTNode *node = makeNode(ASSIGN, "=");
            advance(); /* consume ASSIGN */
            node->left  = lhs;
            node->right = assign_expr();
            return node;
        } else if (match(ADDSUB_ASSIGN)) {
            /* ID ADDSUB_ASSIGN assign_expr */
            char oplex[MAXLEN];
            strcpy(oplex, getLexeme());
            BTNode *lhs = makeNode(ID, savedLex);
            BTNode *node = makeNode(ADDSUB_ASSIGN, oplex);
            advance(); /* consume ADDSUB_ASSIGN */
            node->left  = lhs;
            node->right = assign_expr();
            return node;
        } else {
            /*
              Not an assignment. We consumed the ID token already (advance() was called).
              Now curToken is whatever comes after the ID.
              We need to reconstruct the or_expr starting from this ID.
              Create an ID node, then pass it as the "left" of potential
              further binary ops.  We re-enter the binary-op chain manually.
            */
            BTNode *idNode = makeNode(ID, savedLex);

            /* Continue building muldiv from idNode */
            /* muldiv_expr_tail */
            BTNode *mdLeft = idNode;
            while (!hasError && match(MULDIV)) {
                BTNode *node = makeNode(MULDIV, getLexeme());
                advance();
                node->left  = mdLeft;
                node->right = unary_expr();
                mdLeft = node;
            }
            /* addsub_expr_tail */
            BTNode *asLeft = mdLeft;
            while (!hasError && match(ADDSUB)) {
                BTNode *node = makeNode(ADDSUB, getLexeme());
                advance();
                node->left  = asLeft;
                node->right = muldiv_expr();
                asLeft = node;
            }
            /* and_expr_tail */
            BTNode *andLeft = asLeft;
            while (!hasError && match(AND)) {
                BTNode *node = makeNode(AND, "&");
                advance();
                node->left  = andLeft;
                node->right = addsub_expr();
                andLeft = node;
            }
            /* xor_expr_tail */
            BTNode *xorLeft = andLeft;
            while (!hasError && match(XOR)) {
                BTNode *node = makeNode(XOR, "^");
                advance();
                node->left  = xorLeft;
                node->right = and_expr();
                xorLeft = node;
            }
            /* or_expr_tail */
            BTNode *orLeft = xorLeft;
            while (!hasError && match(OR)) {
                BTNode *node = makeNode(OR, "|");
                advance();
                node->left  = orLeft;
                node->right = xor_expr();
                orLeft = node;
            }
            return orLeft;
        }
    }
    /* No ID at front: fall through to or_expr normally */
    return or_expr();
}

/*============================================================================================
  Code generator

  codeGen(node) returns a register number that holds the computed value.
  Caller is responsible for freeing that register when done.

  We track whether a sub-expression contains any variable (for division-by-zero).
============================================================================================*/

/* Returns 1 if tree contains at least one ID node */
static int hasVariable(BTNode *root) {
    if (!root) return 0;
    if (root->data == ID) return 1;
    return hasVariable(root->left) || hasVariable(root->right);
}

/* Load variable into a new register; return reg number */
static int loadVar(const char *name) {
    int idx = findSym(name);
    if (idx < 0) { hasError = 1; return 0; }
    int r = allocReg();
    emitMOV_RM(r, table[idx].memAddr);
    return r;
}

/* Store register into variable's memory */
static void storeVar(const char *name, int r) {
    int idx = findSym(name);
    if (idx < 0) { hasError = 1; return; }
    emitMOV_MR(table[idx].memAddr, r);
}

/*
  codeGen: generate code for a subtree.
  Returns register holding the result.
  Caller must freeReg() that register when no longer needed.
*/
static int codeGen(BTNode *root) {
    if (!root || hasError) return 0;

    switch (root->data) {

    case INT: {
        int r = allocReg();
        emitMOV_RC(r, atoi(root->lexeme));
        return r;
    }

    case ID: {
        /* Check variable is defined */
        int idx = findSym(root->lexeme);
        if (idx < 0 || !table[idx].defined) {
            err(NOTFOUND);
            return 0;
        }
        return loadVar(root->lexeme);
    }

    case INCDEC: {
        /* ++x or --x: x = x ± 1 then return new value */
        BTNode *idNode = root->left;
        int idx = findSym(idNode->lexeme);
        if (idx < 0 || !table[idx].defined) { err(NOTFOUND); return 0; }

        int r = loadVar(idNode->lexeme);
        int one = allocReg();
        emitMOV_RC(one, 1);
        if (root->lexeme[0] == '+') emitADD(r, one);
        else                        emitSUB(r, one);
        freeReg(one);
        storeVar(idNode->lexeme, r);
        return r;
    }

    case ASSIGN: {
        /* left must be ID (guaranteed by grammar) */
        BTNode *lhs = root->left;
        if (lhs->data != ID) { err(NOTLVAL); return 0; }

        /* Ensure the variable exists in the symbol table */
        int idx = findSym(lhs->lexeme);
        if (idx < 0) idx = addSym(lhs->lexeme);
        /* Mark it defined (LHS of =) */
        table[idx].defined = 1;

        int rr = codeGen(root->right);
        if (hasError) return rr;
        storeVar(lhs->lexeme, rr);
        /* Return a copy so the caller can use the value (e.g. x = y = 3) */
        /* Actually we return rr itself; storeVar doesn't consume it */
        return rr;
    }

    case ADDSUB_ASSIGN: {
        /* left must be ID */
        BTNode *lhs = root->left;
        if (lhs->data != ID) { err(NOTLVAL); return 0; }

        int idx = findSym(lhs->lexeme);
        if (idx < 0 || !table[idx].defined) { err(NOTFOUND); return 0; }

        int lReg = loadVar(lhs->lexeme);
        int rReg = codeGen(root->right);
        if (hasError) { freeReg(lReg); return rReg; }

        if (root->lexeme[0] == '+') emitADD(lReg, rReg);
        else                        emitSUB(lReg, rReg);
        freeReg(rReg);

        storeVar(lhs->lexeme, lReg);
        return lReg;
    }

    case ADDSUB:
    case MULDIV:
    case AND:
    case OR:
    case XOR: {
        int lReg = codeGen(root->left);
        if (hasError) return lReg;
        int rReg = codeGen(root->right);
        if (hasError) { freeReg(lReg); return rReg; }

        if (root->data == MULDIV && root->lexeme[0] == '/') {
            /* Division-by-zero check */
            if (!hasVariable(root->right)) {
                /* purely constant RHS — evaluate statically */
                /* We can't easily evaluate without a separate pass, so we use
                   a conservative approach: emit a runtime check using a temp.
                   However, the spec says "no variable in RHS → invalid expression".
                   We can detect this at parse time by doing a simple constant-fold check.
                   For safety, do it here too: if the register was loaded from an INT node
                   we can check. But we've already generated code. 
                   The cleanest approach per spec: we check hasVariable on the rhs subtree.
                   If false → error at compile time (constant zero is invalid). */
                /* Note: the value in rReg might not be zero (e.g. 2+1=3).
                   But the spec says "no variable in RHS" means it IS invalid if it
                   evaluates to zero. We must detect constant-zero at compile time.
                   Since we can't easily constant-fold after code emission, we do a
                   separate static evaluation pass below. */
                int staticVal = 0, canFold = 1;
                /* small evaluator for constant-only subtrees */
                /* We'll inline a recursive const-eval */
                #define CONSTEVAL(n, out, ok) do { \
                    BTNode *_n = (n); int _v = 0, _ok = 1; \
                    /* just attempt; if has ID → not const */ \
                    if (hasVariable(_n)) { _ok = 0; } \
                    else { \
                        /* evaluate */ \
                        /* We'll use a small helper */ \
                        _v = 0; /* placeholder */ \
                    } \
                    (out) = _v; (ok) = _ok; \
                } while(0)
                /* Actually implement a proper const-eval */
                /* We already know hasVariable is false, so all leaves are INT */
                /* Re-evaluate statically */
                {
                    /* inline const-only evaluator */
                    /* We call constEval later; for now just proceed */
                    (void)staticVal; (void)canFold;
                }
                /* We handle this properly in the wrapper below */
            }
        }

        switch (root->data) {
            case ADDSUB:
                if (root->lexeme[0] == '+') emitADD(lReg, rReg);
                else                        emitSUB(lReg, rReg);
                break;
            case MULDIV:
                if (root->lexeme[0] == '*') emitMUL(lReg, rReg);
                else                        emitDIV(lReg, rReg);
                break;
            case AND: emitAND(lReg, rReg); break;
            case OR:  emitOR (lReg, rReg); break;
            case XOR: emitXOR(lReg, rReg); break;
            default: break;
        }
        freeReg(rReg);
        return lReg;
    }

    default:
        return 0;
    }
}

/*============================================================================================
  Static constant evaluator (for division-by-zero detection on constant-only RHS)
============================================================================================*/
static int constEval(BTNode *root, int *result) {
    /* Returns 1 if successfully evaluated (no variables), sets *result.
       Returns 0 if tree contains variables or division by zero. */
    if (!root) { *result = 0; return 1; }
    if (root->data == INT)  { *result = atoi(root->lexeme); return 1; }
    if (root->data == ID)   { return 0; } /* has variable */
    if (root->data == INCDEC) { return 0; } /* has variable */

    int lv, rv;
    if (!constEval(root->left, &lv))  return 0;
    if (!constEval(root->right, &rv)) return 0;

    switch (root->data) {
        case ADDSUB:
            *result = (root->lexeme[0]=='+') ? lv+rv : lv-rv; return 1;
        case MULDIV:
            if (root->lexeme[0]=='*') { *result = lv*rv; return 1; }
            if (rv == 0) return 0; /* div by zero in constant sub-expr */
            *result = lv/rv; return 1;
        case AND: *result = lv & rv; return 1;
        case OR:  *result = lv | rv; return 1;
        case XOR: *result = lv ^ rv; return 1;
        default:  return 0;
    }
}

/*============================================================================================
  Pre-pass: validate the AST before code generation
  Returns 0 if valid, 1 if error found.
============================================================================================*/

/* Check for invalid division by constant zero */
static int validateDiv(BTNode *root) {
    if (!root) return 0;
    if (root->data == MULDIV && root->lexeme[0] == '/') {
        /* If RHS has no variable, constant-eval it */
        if (!hasVariable(root->right)) {
            int val;
            if (!constEval(root->right, &val)) {
                /* constEval failed: means nested div by zero in RHS */
                return 1;
            }
            if (val == 0) return 1; /* divide by constant zero */
        }
    }
    return validateDiv(root->left) || validateDiv(root->right);
}

/* Check for undefined variables (RHS use before assignment) */
/* Also marks new LHS variables as defined */
static int validateVars(BTNode *root, int isLHS) {
    if (!root || hasError) return 0;

    if (root->data == ASSIGN) {
        /* LHS */
        BTNode *lhs = root->left;
        if (!lhs || lhs->data != ID) return 1; /* not lval */
        /* Check/create LHS variable — mark as defined */
        int idx = findSym(lhs->lexeme);
        if (idx < 0) idx = addSym(lhs->lexeme);
        table[idx].defined = 1;
        /* Validate RHS */
        return validateVars(root->right, 0);
    }
    if (root->data == ADDSUB_ASSIGN) {
        BTNode *lhs = root->left;
        if (!lhs || lhs->data != ID) return 1;
        int idx = findSym(lhs->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return validateVars(root->right, 0);
    }
    if (root->data == ID) {
        if (isLHS) return 0; /* handled by ASSIGN case */
        int idx = findSym(root->lexeme);
        if (idx < 0 || !table[idx].defined) return 1; /* undefined variable */
        return 0;
    }
    if (root->data == INCDEC) {
        BTNode *id = root->left;
        if (!id || id->data != ID) return 1;
        int idx = findSym(id->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return 0;
    }
    return validateVars(root->left, 0) || validateVars(root->right, 0);
}

/*============================================================================================
  Preamble: load x, y, z from memory into registers for final store
  We emit this once at the top of output.
  Actually per spec we just need to STORE final x,y,z into r0,r1,r2 before EXIT 0.
  We don't need a preamble — we handle it at the end.
============================================================================================*/

/*============================================================================================
  statement
============================================================================================*/
static void statement(void) {
    if (match(ENDFILE)) {
        /* Emit final: store x,y,z into r0,r1,r2 */
        /* Reset register allocator — use r0,r1,r2 for final output */
        memset(regUsed, 0, sizeof(regUsed));

        /* Load x -> r0 */
        emitMOV_RM(0, 0);
        /* Load y -> r1 */
        emitMOV_RM(1, 4);
        /* Load z -> r2 */
        emitMOV_RM(2, 8);

        emit("EXIT 0\n");
        totalCycles += 20;

        printf("%s", outBuf);
        exit(0);
    }

    if (match(END)) {
        advance();
        return;
    }

    /* assign_expr END */
    /* Reset register allocator for this statement */
    memset(regUsed, 0, sizeof(regUsed));
    /* Mark r0,r1,r2 as "in use" so temps don't clobber them
       Actually we don't reserve them during computation;
       we'll use them freely and only fix up at EXIT.
       But to keep it simple: use r3..r7 for temporaries,
       reserving r0,r1,r2 for the final x,y,z store.
       Mark them used now. */
    regUsed[0] = regUsed[1] = regUsed[2] = 1;

    BTNode *tree = assign_expr();

    if (hasError) {
        freeTree(tree);
        /* drain to end of line */
        while (!match(END) && !match(ENDFILE)) advance();
        if (match(END)) advance();
        /* output EXIT 1 */
        outPos = 0; outBuf[0] = '\0';
        printf("EXIT 1\n");
        exit(0);
    }

    if (!match(END)) {
        err(SYNTAXERR);
        freeTree(tree);
        while (!match(END) && !match(ENDFILE)) advance();
        if (match(END)) advance();
        outPos = 0; outBuf[0] = '\0';
        printf("EXIT 1\n");
        exit(0);
    }
    advance(); /* consume END */

    /* Validate: undefined variables */
    if (validateVars(tree, 0)) {
        freeTree(tree);
        printf("EXIT 1\n");
        exit(0);
    }

    /* Validate: division by constant zero */
    if (validateDiv(tree)) {
        freeTree(tree);
        printf("EXIT 1\n");
        exit(0);
    }

    /* Generate code */
    int resultReg = codeGen(tree);
    if (hasError) {
        freeTree(tree);
        printf("EXIT 1\n");
        exit(0);
    }
    /* Free the result register (we don't need the top-level result) */
    freeReg(resultReg);

    freeTree(tree);
}

/*============================================================================================
  main
============================================================================================*/
int main(void) {
    initTable();
    while (1) {
        statement();
    }
    return 0;
}