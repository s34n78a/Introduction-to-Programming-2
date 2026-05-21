#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// for lex
#define MAXLEN 256
// Token types
typedef enum {
    UNKNOWN, END, ENDFILE, 
    INT, ID,
    ADDSUB, MULDIV,
    ASSIGN,
    LPAREN, RPAREN,
    // TODO: add new token types
    ADDSUB_ASSIGN,  // += or -=
    INCDEC,         // ++ or --
    AND,            // &
    OR,             // |
    XOR             // ^
} TokenSet;
TokenSet getToken(void);
TokenSet curToken = UNKNOWN;
char lexeme[MAXLEN];
int match(TokenSet token);
void advance(void);
char *getLexeme(void);
// for parser
#define TBLSIZE 256
#define PRINTERR 1
#define error(errorNum) { \
    if (PRINTERR) \
        fprintf(stderr, "error() called at %s:%d: ", __FILE__, __LINE__); \
    err(errorNum); \
}
// Error types
typedef enum {
    UNDEFINED, MISPAREN, NOTNUMID, NOTFOUND, RUNOUT, NOTLVAL, DIVZERO, SYNTAXERR
} ErrorType;
// Structure of the symbol table
typedef struct {
    int val;
    char name[MAXLEN];
    // TODO: add memAddr and defined flag
    int memAddr;
    int defined;
} Symbol;
// Structure of a tree node
typedef struct _Node {
    TokenSet data;
    int val;
    char lexeme[MAXLEN];
    struct _Node *left; 
    struct _Node *right;
} BTNode;
int sbcount = 0;
Symbol table[TBLSIZE];
// TODO: next free memory address for new variables
int nextMem = 12;
// TODO: register allocator state
#define REGCOUNT 8
int regUsed[REGCOUNT];
// TODO: output buffer — collect assembly here, print all at once at EXIT 0
#define OUTBUF 131072
char outBuf[OUTBUF];
int outPos = 0;

void initTable(void);
// TODO: getval now returns the table index (-1 if not found), instead of the variable's value
int getval(char *str);
// TODO: setval now adds a new variable to the table; val param removed
int setval(char *str);
BTNode *makeNode(TokenSet tok, const char *lexe);
void freeTree(BTNode *root);
// TODO: forward-declare all new parser functions
BTNode *factor(void);
BTNode *unary_expr(void);
BTNode *muldiv_expr(void);
BTNode *addsub_expr(void);
BTNode *and_expr(void);
BTNode *xor_expr(void);
BTNode *or_expr(void);
BTNode *assign_expr(void);
BTNode *finish_from_id(BTNode *base);
void statement(void);
void err(ErrorType errorNum);
// TODO: forward-declare emit helpers, allocator, validation, evaluateTree
void emitMOV_RC(int d, int c);
void emitMOV_RM(int d, int a);
void emitMOV_MR(int a, int s);
void emitADD(int d, int s);
void emitSUB(int d, int s);
void emitMUL(int d, int s);
void emitDIV(int d, int s);
void emitAND(int d, int s);
void emitOR (int d, int s);
void emitXOR(int d, int s);
int  allocReg(void);
void freeReg(int r);
int  hasVariable(BTNode *r);
int  constEval(BTNode *r, int *res);
int  validateDiv(BTNode *r);
int  validateVars(BTNode *r);
int  evaluateTree(BTNode *root);

/*============================================================================================
lex implementation
============================================================================================*/
TokenSet getToken(void)
{
    int i = 0;
    char c = '\0';
    while ((c = fgetc(stdin)) == ' ' || c == '\t');
    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i < MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    // TODO: support multi-character IDs and underscore
    } else if (isalpha(c) || c == '_') {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while ((isalnum(c) || c == '_') && i < MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    // TODO: peek one char ahead to detect ++ / -- and += / -=
    } else if (c == '+' || c == '-') {
        char nx = fgetc(stdin);
        if (nx == c)   { lexeme[0]=c; lexeme[1]=c;   lexeme[2]='\0'; return INCDEC; }
        if (nx == '=') { lexeme[0]=c; lexeme[1]='='; lexeme[2]='\0'; return ADDSUB_ASSIGN; }
        ungetc(nx, stdin);
        lexeme[0] = c; lexeme[1] = '\0';
        return ADDSUB;
    } else if (c == '*' || c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULDIV;
    } else if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    // TODO: add bitwise operator tokens
    } else if (c == '&') { strcpy(lexeme, "&"); return AND;
    } else if (c == '|') { strcpy(lexeme, "|"); return OR;
    } else if (c == '^') { strcpy(lexeme, "^"); return XOR;
    } else if (c == EOF) {
        return ENDFILE;
    } else {
        return UNKNOWN;
    }
}
void advance(void) {
    curToken = getToken();
}
int match(TokenSet token) {
    if (curToken == UNKNOWN)
        advance();
    return token == curToken;
}
char *getLexeme(void) {
    return lexeme;
}
/*============================================================================================
parser implementation
============================================================================================*/
void initTable(void) {
    strcpy(table[0].name, "x");
    table[0].val = 0;
    // TODO: assign memory addresses and mark x/y/z as defined
    table[0].memAddr = 0;  table[0].defined = 1;
    strcpy(table[1].name, "y");
    table[1].val = 0;
    table[1].memAddr = 4;  table[1].defined = 1;
    strcpy(table[2].name, "z");
    table[2].val = 0;
    table[2].memAddr = 8;  table[2].defined = 1;
    sbcount = 3;
}
// TODO: getval — returns table index of str, or -1 if not found
//       (original returned the variable's value and auto-created missing entries)
int getval(char *str) {
    int i = 0;
    for (i = 0; i < sbcount; i++)
        if (strcmp(str, table[i].name) == 0)
            return i;
    return -1;
}
// TODO: setval — appends a new undefined variable and returns its index
//       (original updated an existing variable's value and took a val parameter)
int setval(char *str) {
    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    strcpy(table[sbcount].name, str);
    table[sbcount].memAddr = nextMem;
    table[sbcount].defined = 0;
    nextMem += 4;
    return sbcount++;
}
BTNode *makeNode(TokenSet tok, const char *lexe) {
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    strcpy(node->lexeme, lexe);
    node->data = tok;
    node->val = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}
void freeTree(BTNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// TODO: replace factor/term/expr with the full grammar.
//       New call chain: assign_expr -> or_expr -> xor_expr -> and_expr
//                    -> addsub_expr -> muldiv_expr -> unary_expr -> factor

// factor := INT | ID | INCDEC ID | LPAREN assign_expr RPAREN
BTNode *factor(void) {
    BTNode *retp = NULL;
    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
        return retp;
    } else if (match(ID)) {
        retp = makeNode(ID, getLexeme());
        advance();
        return retp;
    // TODO: handle INCDEC ID (e.g. ++x or --x)
    } else if (match(INCDEC)) {
        char op[MAXLEN]; strcpy(op, getLexeme()); advance();
        if (!match(ID)) { error(NOTNUMID); }
        retp = makeNode(INCDEC, op);
        retp->left = makeNode(ID, getLexeme());
        advance();
        return retp;
    } else if (match(LPAREN)) {
        advance();
        // TODO: recurse into assign_expr instead of expr
        retp = assign_expr();
        if (match(RPAREN)) advance();
        else error(MISPAREN);
        return retp;
    } else {
        error(NOTNUMID);
        return makeNode(INT, "0");
    }
}
// TODO: unary_expr — handles unary +/- as prefix (builds 0 - x style node)
// unary_expr := ADDSUB unary_expr | factor
BTNode *unary_expr(void) {
    BTNode *node;
    if (match(ADDSUB)) {
        char op[MAXLEN]; strcpy(op, getLexeme());
        node = makeNode(ADDSUB, op);
        node->left = makeNode(INT, "0");
        advance();
        node->right = unary_expr();
        return node;
    }
    return factor();
}
// TODO: muldiv_expr — same structure as old term(), but calls unary_expr
// muldiv_expr := unary_expr muldiv_expr_tail
// muldiv_expr_tail := MULDIV unary_expr muldiv_expr_tail | NiL
BTNode *muldiv_expr(void) {
    BTNode *left, *node;
    left = unary_expr();
    while (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme()); advance();
        node->left = left; node->right = unary_expr(); left = node;
    }
    return left;
}
// TODO: addsub_expr — same structure as old expr(), but calls muldiv_expr
// addsub_expr := muldiv_expr addsub_expr_tail
// addsub_expr_tail := ADDSUB muldiv_expr addsub_expr_tail | NiL
BTNode *addsub_expr(void) {
    BTNode *left, *node;
    left = muldiv_expr();
    while (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme()); advance();
        node->left = left; node->right = muldiv_expr(); left = node;
    }
    return left;
}
// TODO: and_expr — new precedence level for &
// and_expr := addsub_expr and_expr_tail
// and_expr_tail := AND addsub_expr and_expr_tail | NiL
BTNode *and_expr(void) {
    BTNode *left, *node;
    left = addsub_expr();
    while (match(AND)) {
        node = makeNode(AND, "&"); advance();
        node->left = left; node->right = addsub_expr(); left = node;
    }
    return left;
}
// TODO: xor_expr — new precedence level for ^
// xor_expr := and_expr xor_expr_tail
// xor_expr_tail := XOR and_expr xor_expr_tail | NiL
BTNode *xor_expr(void) {
    BTNode *left, *node;
    left = and_expr();
    while (match(XOR)) {
        node = makeNode(XOR, "^"); advance();
        node->left = left; node->right = and_expr(); left = node;
    }
    return left;
}
// TODO: or_expr — new precedence level for |
// or_expr := xor_expr or_expr_tail
// or_expr_tail := OR xor_expr or_expr_tail | NiL
BTNode *or_expr(void) {
    BTNode *left, *node;
    left = xor_expr();
    while (match(OR)) {
        node = makeNode(OR, "|"); advance();
        node->left = left; node->right = xor_expr(); left = node;
    }
    return left;
}
// TODO: finish_from_id — re-enters the full binary-op chain when we have already
//       consumed an ID and found it is NOT followed by = or +=/-=
BTNode *finish_from_id(BTNode *base) {
    BTNode *left = base, *node;
    while (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme()); advance();
        node->left = left; node->right = unary_expr(); left = node;
    }
    while (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme()); advance();
        node->left = left; node->right = muldiv_expr(); left = node;
    }
    while (match(AND)) {
        node = makeNode(AND, "&"); advance();
        node->left = left; node->right = addsub_expr(); left = node;
    }
    while (match(XOR)) {
        node = makeNode(XOR, "^"); advance();
        node->left = left; node->right = and_expr(); left = node;
    }
    while (match(OR)) {
        node = makeNode(OR, "|"); advance();
        node->left = left; node->right = xor_expr(); left = node;
    }
    return left;
}
// TODO: assign_expr — top-level rule; peek past ID to detect = or +=/-=
// assign_expr := ID ASSIGN assign_expr | ID ADDSUB_ASSIGN assign_expr | or_expr
BTNode *assign_expr(void) {
    BTNode *node, *lhs;
    char savedLex[MAXLEN], oplex[MAXLEN];
    if (match(ID)) {
        strcpy(savedLex, getLexeme()); advance();
        if (match(ASSIGN)) {
            lhs = makeNode(ID, savedLex); node = makeNode(ASSIGN, "=");
            advance(); node->left = lhs; node->right = assign_expr(); return node;
        }
        if (match(ADDSUB_ASSIGN)) {
            strcpy(oplex, getLexeme());
            lhs = makeNode(ID, savedLex); node = makeNode(ADDSUB_ASSIGN, oplex);
            advance(); node->left = lhs; node->right = assign_expr(); return node;
        }
        return finish_from_id(makeNode(ID, savedLex));
    }
    return or_expr();
}

void err(ErrorType errorNum) {
    if (PRINTERR) {
        fprintf(stderr, "error: ");
        switch (errorNum) {
            case MISPAREN:  fprintf(stderr, "mismatched parenthesis\n");        break;
            case NOTNUMID:  fprintf(stderr, "number or identifier expected\n"); break;
            case NOTFOUND:  fprintf(stderr, "variable not defined\n");          break;
            case RUNOUT:    fprintf(stderr, "out of memory\n");                 break;
            case NOTLVAL:   fprintf(stderr, "lvalue required as an operand\n"); break;
            case DIVZERO:   fprintf(stderr, "divide by constant zero\n");       break;
            case SYNTAXERR: fprintf(stderr, "syntax error\n");                  break;
            default:        fprintf(stderr, "undefined error\n");               break;
        }
    }
    // TODO: print EXIT 1 instead of just exit(0)
    printf("EXIT 1\n");
    exit(0);
}
/*============================================================================================
codeGen implementation
============================================================================================*/
// TODO: emit helpers — each writes directly into outBuf with sprintf, no stdarg needed
void emitMOV_RC(int d, int c) { outPos += sprintf(outBuf+outPos, "MOV r%d %d\n",   d, c); }
void emitMOV_RM(int d, int a) { outPos += sprintf(outBuf+outPos, "MOV r%d [%d]\n", d, a); }
void emitMOV_MR(int a, int s) { outPos += sprintf(outBuf+outPos, "MOV [%d] r%d\n", a, s); }
void emitADD(int d, int s)    { outPos += sprintf(outBuf+outPos, "ADD r%d r%d\n",   d, s); }
void emitSUB(int d, int s)    { outPos += sprintf(outBuf+outPos, "SUB r%d r%d\n",   d, s); }
void emitMUL(int d, int s)    { outPos += sprintf(outBuf+outPos, "MUL r%d r%d\n",   d, s); }
void emitDIV(int d, int s)    { outPos += sprintf(outBuf+outPos, "DIV r%d r%d\n",   d, s); }
void emitAND(int d, int s)    { outPos += sprintf(outBuf+outPos, "AND r%d r%d\n",   d, s); }
void emitOR (int d, int s)    { outPos += sprintf(outBuf+outPos, "OR r%d r%d\n",    d, s); }
void emitXOR(int d, int s)    { outPos += sprintf(outBuf+outPos, "XOR r%d r%d\n",   d, s); }
// TODO: allocReg/freeReg — simple lowest-free-register allocator
int allocReg(void) {
    int i;
    for (i = 0; i < REGCOUNT; i++)
        if (!regUsed[i]) { regUsed[i] = 1; return i; }
    fprintf(stderr, "FATAL: out of registers\n"); exit(1);
}
void freeReg(int r) { if (r >= 0 && r < REGCOUNT) regUsed[r] = 0; }

// TODO: validation helpers
// hasVariable — returns 1 if the subtree contains any ID node
int hasVariable(BTNode *r) {
    if (!r) return 0;
    if (r->data == ID) return 1;
    return hasVariable(r->left) || hasVariable(r->right);
}
// constEval — statically evaluates a constant-only subtree
// returns 1 and sets *res on success, 0 if tree has variables or hits div-by-zero
int constEval(BTNode *r, int *res) {
    int lv, rv;
    if (!r)             { *res = 0; return 1; }
    if (r->data == INT) { *res = atoi(r->lexeme); return 1; }
    if (r->data == ID || r->data == INCDEC) return 0;
    if (!constEval(r->left,  &lv)) return 0;
    if (!constEval(r->right, &rv)) return 0;
    switch (r->data) {
        case ADDSUB: *res = (r->lexeme[0] == '+') ? lv + rv : lv - rv; return 1;
        case MULDIV:
            if (r->lexeme[0] == '*') { *res = lv * rv; return 1; }
            if (rv == 0) return 0;
            *res = lv / rv; return 1;
        case AND: *res = lv & rv; return 1;
        case OR:  *res = lv | rv; return 1;
        case XOR: *res = lv ^ rv; return 1;
        default: return 0;
    }
}
// validateDiv — returns 1 if any division by constant zero is found
int validateDiv(BTNode *r) {
    int val;
    if (!r) return 0;
    if (r->data == MULDIV && r->lexeme[0] == '/') {
        if (!hasVariable(r->right)) {
            if (!constEval(r->right, &val) || val == 0) return 1;
        }
    }
    return validateDiv(r->left) || validateDiv(r->right);
}
// validateVars — returns 1 if any variable is used before being defined
int validateVars(BTNode *r) {
    int idx;
    if (!r) return 0;
    if (r->data == ASSIGN) {
        BTNode *lhs = r->left;
        if (!lhs || lhs->data != ID) return 1;
        idx = getval(lhs->lexeme);
        if (idx < 0) idx = setval(lhs->lexeme);
        if (idx < 0) return 1;
        table[idx].defined = 1;
        return validateVars(r->right);
    }
    if (r->data == ADDSUB_ASSIGN) {
        BTNode *lhs = r->left;
        if (!lhs || lhs->data != ID) return 1;
        idx = getval(lhs->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return validateVars(r->right);
    }
    if (r->data == ID) {
        idx = getval(r->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return 0;
    }
    if (r->data == INCDEC) {
        BTNode *id = r->left;
        if (!id || id->data != ID) return 1;
        idx = getval(id->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return 0;
    }
    return validateVars(r->left) || validateVars(r->right);
}
// TODO: evaluateTree — same recursive structure as original, but:
//       lv/rv now hold register numbers instead of computed values,
//       and each case emits assembly instead of computing arithmetic.
int evaluateTree(BTNode *root) {
    int retval = 0, lv = 0, rv = 0;
    if (root != NULL) {
        switch (root->data) {
            case ID:
                // TODO: load variable from memory into a new register
                retval = allocReg();
                emitMOV_RM(retval, table[getval(root->lexeme)].memAddr);
                break;
            case INT:
                // TODO: load constant into a new register
                retval = allocReg();
                emitMOV_RC(retval, atoi(root->lexeme));
                break;
            // TODO: INCDEC — load variable, add/sub 1, write back, return updated reg
            case INCDEC:
                retval = allocReg();
                emitMOV_RM(retval, table[getval(root->left->lexeme)].memAddr);
                rv = allocReg();
                emitMOV_RC(rv, 1);
                if (root->lexeme[0] == '+') emitADD(retval, rv);
                else                        emitSUB(retval, rv);
                freeReg(rv);
                emitMOV_MR(table[getval(root->left->lexeme)].memAddr, retval);
                break;
            case ASSIGN:
                // TODO: generate RHS, store result into LHS memory address
                rv = evaluateTree(root->right);
                emitMOV_MR(table[getval(root->left->lexeme)].memAddr, rv);
                retval = rv;  // return rv so chained assigns (x = y = 3) work
                break;
            // TODO: ADDSUB_ASSIGN — load LHS, compute RHS, add/sub, write back
            case ADDSUB_ASSIGN:
                lv = allocReg();
                emitMOV_RM(lv, table[getval(root->left->lexeme)].memAddr);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+=") == 0) emitADD(lv, rv);
                else                                 emitSUB(lv, rv);
                freeReg(rv);
                emitMOV_MR(table[getval(root->left->lexeme)].memAddr, lv);
                retval = lv;
                break;
            case ADDSUB:
            case MULDIV:
            // TODO: add AND, OR, XOR — same structure as ADDSUB/MULDIV
            case AND:
            case OR:
            case XOR:
                lv = evaluateTree(root->left);
                rv = evaluateTree(root->right);
                // TODO: emit instructions instead of computing values
                if      (strcmp(root->lexeme, "+")  == 0) emitADD(lv, rv);
                else if (strcmp(root->lexeme, "-")  == 0) emitSUB(lv, rv);
                else if (strcmp(root->lexeme, "*")  == 0) emitMUL(lv, rv);
                else if (strcmp(root->lexeme, "/")  == 0) emitDIV(lv, rv);
                else if (strcmp(root->lexeme, "&")  == 0) emitAND(lv, rv);
                else if (strcmp(root->lexeme, "|")  == 0) emitOR (lv, rv);
                else if (strcmp(root->lexeme, "^")  == 0) emitXOR(lv, rv);
                freeReg(rv);  // TODO: free RHS register, result lives in lv
                retval = lv;
                break;
            default:
                retval = 0;
        }
    }
    return retval;
}
void printPrefix(BTNode *root) {
    if (root != NULL) {
        printf("%s ", root->lexeme);
        printPrefix(root->left);
        printPrefix(root->right);
    }
}
/*============================================================================================
main
============================================================================================*/
// TODO: rewrite statement() to call assign_expr, validate, then evaluateTree
void statement(void) {
    BTNode *retp = NULL;
    int resultReg;
    if (match(ENDFILE)) {
        // TODO: store final x,y,z into r0,r1,r2 then print buffered output
        memset(regUsed, 0, sizeof(regUsed));
        emitMOV_RM(0, 0); emitMOV_RM(1, 4); emitMOV_RM(2, 8);
        outPos += sprintf(outBuf+outPos, "EXIT 0\n");
        printf("%s", outBuf);
        exit(0);
    } else if (match(END)) {
        advance();
    } else {
        // TODO: reset register allocator for each new statement
        memset(regUsed, 0, sizeof(regUsed));
        // TODO: call assign_expr instead of expr
        retp = assign_expr();
        if (match(END)) {
            // TODO: validate then generate code instead of evaluating
            if (validateVars(retp)) err(NOTFOUND);
            if (validateDiv(retp))  err(DIVZERO);
            memset(regUsed, 0, sizeof(regUsed));
            resultReg = evaluateTree(retp);
            freeReg(resultReg);
            freeTree(retp);
            advance();
        } else {
            error(SYNTAXERR);
        }
    }
}
int main() {
    initTable();
    while (1) {
        statement();
    }
    return 0;
}