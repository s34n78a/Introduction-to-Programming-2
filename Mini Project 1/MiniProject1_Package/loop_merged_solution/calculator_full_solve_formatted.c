#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h> // TO DO: header file for variable argument functions (va_list, va_start, va_end, etc.)

// for lex
#define MAXLEN 256

// Token types
typedef enum {
    UNKNOWN, END, ENDFILE, 
    INT, ID,
    ADDSUB, MULDIV,
    ASSIGN, 
    LPAREN, RPAREN,
    // TO DO: add more token types if needed
    ADDSUB_ASSIGN,/* += or -= */
    INCDEC,       /* ++ or -- */
    AND,          /* & */
    OR,           /* | */
    XOR           /* ^ */
} TokenSet;

TokenSet getToken(void);
TokenSet curToken = UNKNOWN;
char lexeme[MAXLEN];

// Test if a token matches the current token 
int match(TokenSet token);
// Get the next token
void advance(void);
// Get the lexeme of the current token
char *getLexeme(void);


// for parser
#define TBLSIZE 256
// Set PRINTERR to 1 to print error message while calling error()
// Make sure you set PRINTERR to 0 before you submit your code
#define PRINTERR 0

// Call this macro to print error message and exit the program
// This will also print where you called it in your program
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
    char name[MAXLEN];
    // TO DO: add more fields if needed, such as defined/undefined, memory address, etc.
    int memAddr;    /* memory address (-1 = not in memory yet) */
    int defined;    /* 1 = has been assigned at least once (or is x/y/z) */
} Symbol;

// Structure of a tree node
typedef struct _Node {
    TokenSet data;
    char lexeme[MAXLEN];
    struct _Node *left; 
    struct _Node *right;
} BTNode;

int sbcount = 0;
Symbol table[TBLSIZE];

// Initialize the symbol table with builtin variables
void initTable(void);
// Make a new node according to token type and lexeme
BTNode *makeNode(TokenSet tok, const char *lexe);
// Free the syntax tree
void freeTree(BTNode *root);
// TO DO: update parser function declarations to match new precedence hierarchy
BTNode *factor(void);
BTNode *unary_expr(void);
BTNode *muldiv_expr(void);    /* replaces term() */
BTNode *addsub_expr(void);    /* replaces expr() */
BTNode *and_expr(void);
BTNode *xor_expr(void);
BTNode *or_expr(void);
BTNode *finish_from_id(BTNode *base);
BTNode *assign_expr(void);    /* top-level expression, replaces expr() in statement */
void statement(void);
// Print error message and exit the program
void err(ErrorType errorNum);


// for codeGen
// TO DO: replace evaluateTree/printPrefix with codeGen (compile mode)
int codeGen(BTNode *root);


/*============================================================================================
  TO DO: Definitions
============================================================================================*/

// add variables for memory management, register allocation, etc.
#define REGCOUNT 8   /* r0..r7 */
/* Register allocator: simple "next free register" (never spill in this impl) */
static int    regUsed[REGCOUNT];   /* 0 = free */

/* Next available memory address for new variables (x=0,y=4,z=8, new starts at 12) */
static int    nextMem = 12;

/* Output buffer so we can prepend MOV r0,[0] etc. at the very top once */
#define OUTBUF 131072
static char outBuf[OUTBUF];
static int  outPos = 0;
static int hasError = 0; /* Flag: error encountered */


/*============================================================================================
  TO DO: Output helpers
============================================================================================*/

static void emit(const char *fmt, ...) {
    va_list ap; 
    char tmp[512];
    va_start(ap, fmt); 
    vsnprintf(tmp, sizeof(tmp), fmt, ap); 
    va_end(ap);
    int len = (int)strlen(tmp);
    if (outPos + len < OUTBUF - 1) {
        memcpy(outBuf + outPos, tmp, len);
        outPos += len; 
        outBuf[outPos] = '\0';
    }
}

static void emitMOV_RR(int d,int s) { // TO DO: move from register s to register d
    emit("MOV r%d r%d\n",d,s);
}

static void emitMOV_RC(int d,int c) { // TO DO: move constant c to register d
    emit("MOV r%d %d\n",d,c);
}

static void emitMOV_RM(int d,int a) { // TO DO: move from memory address a to register d
    emit("MOV r%d [%d]\n",d,a);
}

static void emitMOV_MR(int a,int s) { // TO DO: move from register s to memory address a
    emit("MOV [%d] r%d\n",a,s);
}

static void emitADD(int d,int s) { // TO DO: add register s to register d, result in d
    emit("ADD r%d r%d\n",d,s);
}

static void emitSUB(int d,int s) { // TO DO: subtract register s from register d, result in d
    emit("SUB r%d r%d\n",d,s);
}

static void emitMUL(int d,int s) { // TO DO: multiply register d by register s, result in d
    emit("MUL r%d r%d\n",d,s);
}

static void emitDIV(int d,int s) { // TO DO: divide register d by register s, result in d
    emit("DIV r%d r%d\n",d,s);
}

static void emitAND(int d,int s) { // TO DO: bitwise AND register d with register s, result in d
    emit("AND r%d r%d\n",d,s);
}

static void emitOR (int d,int s) { // TO DO: bitwise OR register d with register s, result in d 
    emit("OR r%d r%d\n",d,s);
}

static void emitXOR(int d,int s) { // TO DO: bitwise XOR register d with register s, result in d
    emit("XOR r%d r%d\n",d,s);
}

/*============================================================================================
  TO DO: Register allocator
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


/*============================================================================================
  TO DO: Symbol table
============================================================================================*/
// TO DO: getval/setval replaced by findSym/addSym only (no value tracking in compile mode)
static int findSym(const char *n) { /* Return index in table, or -1 if not found */
    for (int i = 0; i < sbcount; i++) {
        if (strcmp(table[i].name, n) == 0) {
            return i;
        }
    }
    return -1;
}

static int addSym(const char *n) {
    if (sbcount >= TBLSIZE) {
        hasError = 1;
        return -1;
    }
    strcpy(table[sbcount].name, n); 
    table[sbcount].memAddr = nextMem;
    table[sbcount].defined = 0; 
    nextMem += 4; 
    return sbcount++;
}


/*============================================================================================
lex implementation
============================================================================================*/

TokenSet getToken(void)
{
    int i = 0;
    char c = '\0';

    while ((c = fgetc(stdin)) == ' ' || c == '\t');

    // TO DO: check EOF and newline first before other cases
    if (c == (char)EOF) {
        lexeme[0] = '\0';
        return ENDFILE;
    }
    if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    }
    if (isdigit((unsigned char)c)) {
        lexeme[i++] = c;
        while (isdigit((unsigned char)(c = (char)fgetc(stdin))) && i < MAXLEN - 1)
            lexeme[i++] = c;
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    } else if (isalpha((unsigned char)c) || c == '_') { // TO DO: support multi-char identifiers and underscore
        lexeme[i++] = c;
        while ((isalnum((unsigned char)(c = (char)fgetc(stdin))) || c == '_') && i < MAXLEN - 1)
            lexeme[i++] = c;
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    } else if (c == '+' || c == '-') {
        // TO DO: detect ++ / -- (INCDEC) and += / -= (ADDSUB_ASSIGN)
        char nx = (char)fgetc(stdin);
        if (nx == c) {
            lexeme[0] = c; lexeme[1] = c; lexeme[2] = '\0';
            return INCDEC;
        }
        if (nx == '=') {
            lexeme[0] = c; lexeme[1] = '='; lexeme[2] = '\0';
            return ADDSUB_ASSIGN;
        }
        ungetc(nx, stdin);
        lexeme[0] = c; 
        lexeme[1] = '\0';
        return ADDSUB;
    } else if (c == '*' || c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULDIV;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    }
    // TO DO: add bitwise tokens
    else if (c == '&') {
        strcpy(lexeme, "&");
        return AND;
    } else if (c == '|') {
        strcpy(lexeme, "|");
        return OR;
    } else if (c == '^') {
        strcpy(lexeme, "^");
        return XOR;
    } else {
        lexeme[0] = c;
        lexeme[1] = '\0';
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
    table[0].memAddr = 0; // TO DO: assign memory address for x
    table[0].defined = 1; // Mark x as defined since it's a builtin variable
    strcpy(table[1].name, "y");
    table[1].memAddr = 4; // TO DO: assign memory address for y
    table[1].defined = 1; // Mark y as defined since it's a builtin variable
    strcpy(table[2].name, "z");
    table[2].memAddr = 8; // TO DO: assign memory address for z
    table[2].defined = 1; // Mark z as defined since it's a builtin variable
    sbcount = 3;
    nextMem = 12; // TO DO: set nextMem to the next available memory address for new variables
}

BTNode *makeNode(TokenSet tok, const char *lexe) {
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    strcpy(node->lexeme, lexe);
    node->data = tok;
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

// TO DO: drain rest of line on error (compile mode: no interactive prompt recovery)
static void drainLine(void) {
    while (!match(END) && !match(ENDFILE)) advance();
    if (match(END)) advance();
}

void err(ErrorType errorNum) {
    (void)errorNum;
    drainLine();
    hasError = 1;
}

// factor := INT | ID | INCDEC ID | LPAREN assign_expr RPAREN
// TO DO: factor no longer handles ADDSUB (moved to unary_expr) or ASSIGN (moved to assign_expr)
BTNode *factor(void) {
    BTNode *retp = NULL;

    if (hasError) return makeNode(INT, "0");
    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
        return retp;
    }
    if (match(ID)) {
        retp = makeNode(ID, getLexeme());
        advance();
        return retp;
    }
    if (match(INCDEC)) { // TO DO: handle prefix increment/decrement
        char op[MAXLEN];
        strcpy(op, getLexeme());
        advance();
        if (!match(ID)) { err(NOTNUMID); return makeNode(INT, "0"); }
        retp = makeNode(INCDEC, op);
        retp->left = makeNode(ID, getLexeme());
        advance();
        return retp;
    }
    if (match(LPAREN)) {
        advance();
        retp = assign_expr();
        if (!hasError) {
            if (match(RPAREN)) advance();
            else err(MISPAREN);
        }
        return retp;
    }
    err(NOTNUMID);
    return makeNode(INT, "0");
}

// TO DO: unary_expr handles unary ADDSUB (was mixed into factor before)
// unary_expr := ADDSUB unary_expr | factor
BTNode *unary_expr(void) {
    BTNode *node = NULL;
    if (hasError) return makeNode(INT, "0");
    if (match(ADDSUB)) {
        char op[MAXLEN];
        strcpy(op, getLexeme());
        node = makeNode(ADDSUB, op);
        node->left = makeNode(INT, "0");
        advance();
        node->right = unary_expr();
        return node;
    }
    return factor();
}

// muldiv_expr := unary_expr (MULDIV unary_expr)*
// TO DO: replaces term(); now calls unary_expr instead of factor()
BTNode *muldiv_expr(void) {
    BTNode *retp = NULL, *left = NULL;
    if (hasError) return makeNode(INT, "0");
    left = unary_expr();
    while (!hasError && match(MULDIV)) {
        retp = makeNode(MULDIV, getLexeme());
        advance();
        retp->left = left;
        retp->right = unary_expr();
        left = retp;
    }
    return left;
}

// addsub_expr := muldiv_expr (ADDSUB muldiv_expr)*
// TO DO: replaces expr(); now calls muldiv_expr instead of term()
BTNode *addsub_expr(void) {
    BTNode *retp = NULL, *left = NULL;
    if (hasError) return makeNode(INT, "0");
    left = muldiv_expr();
    while (!hasError && match(ADDSUB)) {
        retp = makeNode(ADDSUB, getLexeme());
        advance();
        retp->left = left;
        retp->right = muldiv_expr();
        left = retp;
    }
    return left;
}

// TO DO: new precedence levels for bitwise operators (AND < XOR < OR, all below ADDSUB)
// and_expr := addsub_expr (AND addsub_expr)*
BTNode *and_expr(void) {
    BTNode *retp = NULL, *left = NULL;
    if (hasError) return makeNode(INT, "0");
    left = addsub_expr();
    while (!hasError && match(AND)) {
        retp = makeNode(AND, "&");
        advance();
        retp->left = left;
        retp->right = addsub_expr();
        left = retp;
    }
    return left;
}

// xor_expr := and_expr (XOR and_expr)*
BTNode *xor_expr(void) {
    BTNode *retp = NULL, *left = NULL;
    if (hasError) return makeNode(INT, "0");
    left = and_expr();
    while (!hasError && match(XOR)) {
        retp = makeNode(XOR, "^");
        advance();
        retp->left = left;
        retp->right = and_expr();
        left = retp;
    }
    return left;
}

// or_expr := xor_expr (OR xor_expr)*
BTNode *or_expr(void) {
    BTNode *retp = NULL, *left = NULL;
    if (hasError) return makeNode(INT, "0");
    left = xor_expr();
    while (!hasError && match(OR)) {
        retp = makeNode(OR, "|");
        advance();
        retp->left = left;
        retp->right = xor_expr();
        left = retp;
    }
    return left;
}

// TO DO: finish_from_id continues binary operator chain after an ID has already been consumed
// Needed because assign_expr peeks at the next token after ID before deciding to assign or not
BTNode *finish_from_id(BTNode *base) {
    BTNode *left = base, *node = NULL;
    while (!hasError && match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme()); advance();
        node->left = left; node->right = unary_expr(); left = node;
    }
    while (!hasError && match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme()); advance();
        node->left = left; node->right = muldiv_expr(); left = node;
    }
    while (!hasError && match(AND)) {
        node = makeNode(AND, "&"); advance();
        node->left = left; node->right = addsub_expr(); left = node;
    }
    while (!hasError && match(XOR)) {
        node = makeNode(XOR, "^"); advance();
        node->left = left; node->right = and_expr(); left = node;
    }
    while (!hasError && match(OR)) {
        node = makeNode(OR, "|"); advance();
        node->left = left; node->right = xor_expr(); left = node;
    }
    return left;
}

// TO DO: assign_expr is the new top-level expression rule (replaces expr() in statement)
// assign_expr := ID ASSIGN assign_expr
//              | ID ADDSUB_ASSIGN assign_expr
//              | or_expr
BTNode *assign_expr(void) {
    BTNode *node = NULL, *lhs = NULL;
    char savedLex[MAXLEN], oplex[MAXLEN];
    if (hasError) return makeNode(INT, "0");
    if (match(ID)) {
        strcpy(savedLex, getLexeme());
        advance();
        if (match(ASSIGN)) {
            lhs = makeNode(ID, savedLex);
            node = makeNode(ASSIGN, "=");
            advance();
            node->left = lhs;
            node->right = assign_expr();
            return node;
        }
        if (match(ADDSUB_ASSIGN)) { // TO DO: handle += and -=
            strcpy(oplex, getLexeme());
            lhs = makeNode(ID, savedLex);
            node = makeNode(ADDSUB_ASSIGN, oplex);
            advance();
            node->left = lhs;
            node->right = assign_expr();
            return node;
        }
        return finish_from_id(makeNode(ID, savedLex));
    }
    return or_expr();
}


/*============================================================================================
  TO DO: Validation helpers
============================================================================================*/

// TO DO: check if a subtree contains any variable reference
static int hasVariable(BTNode *r) {
    if (!r) return 0;
    if (r->data == ID) return 1;
    return hasVariable(r->left) || hasVariable(r->right);
}

// TO DO: try to constant-fold a subtree; returns 1 on success and writes result to *res
static int constEval(BTNode *r, int *res) {
    int lv, rv;
    if (!r) { *res = 0; return 1; }
    if (r->data == INT) { *res = atoi(r->lexeme); return 1; }
    if (r->data == ID || r->data == INCDEC) return 0;
    if (!constEval(r->left, &lv)) return 0;
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

// TO DO: walk the tree and detect any division by a constant zero
static int validateDiv(BTNode *r) {
    int val;
    if (!r) return 0;
    if (r->data == MULDIV && r->lexeme[0] == '/') {
        if (!hasVariable(r->right)) {
            if (!constEval(r->right, &val) || val == 0) return 1;
        }
    }
    return validateDiv(r->left) || validateDiv(r->right);
}

// TO DO: walk the tree and detect uses of undefined variables;
//        marks variables as defined when an ASSIGN or ADDSUB_ASSIGN is seen
static int validateVars(BTNode *r) {
    int idx;
    if (!r) return 0;
    if (r->data == ASSIGN) {
        BTNode *lhs = r->left;
        if (!lhs || lhs->data != ID) return 1;
        idx = findSym(lhs->lexeme);
        if (idx < 0) idx = addSym(lhs->lexeme);
        if (idx < 0) return 1;
        table[idx].defined = 1;
        return validateVars(r->right);
    }
    if (r->data == ADDSUB_ASSIGN) {
        BTNode *lhs = r->left;
        if (!lhs || lhs->data != ID) return 1;
        idx = findSym(lhs->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return validateVars(r->right);
    }
    if (r->data == ID) {
        idx = findSym(r->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return 0;
    }
    if (r->data == INCDEC) {
        BTNode *id = r->left;
        if (!id || id->data != ID) return 1;
        idx = findSym(id->lexeme);
        if (idx < 0 || !table[idx].defined) return 1;
        return 0;
    }
    return validateVars(r->left) || validateVars(r->right);
}


/*============================================================================================
codeGen implementation
============================================================================================*/

// TO DO: codeGen replaces evaluateTree — emits assembly instructions instead of evaluating
// Returns the register number holding the result
int codeGen(BTNode *root) {
    int r, lReg, rReg, idx, one;
    BTNode *lhs, *id;
    if (!root || hasError) return 0;
    switch (root->data) {
        case INT:
            r = allocReg();
            emitMOV_RC(r, atoi(root->lexeme));
            return r;
        case ID:
            idx = findSym(root->lexeme);
            r = allocReg();
            emitMOV_RM(r, table[idx].memAddr);
            return r;
        case INCDEC: // TO DO: prefix ++ / -- : load, add/sub 1, store back, return updated value
            id = root->left;
            idx = findSym(id->lexeme);
            r = allocReg();
            emitMOV_RM(r, table[idx].memAddr);
            one = allocReg();
            emitMOV_RC(one, 1);
            if (root->lexeme[0] == '+') emitADD(r, one); else emitSUB(r, one);
            freeReg(one);
            emitMOV_MR(table[idx].memAddr, r);
            return r;
        case ASSIGN: // TO DO: evaluate RHS, store to LHS variable's memory address
            lhs = root->left;
            idx = findSym(lhs->lexeme);
            rReg = codeGen(root->right);
            if (hasError) return rReg;
            emitMOV_MR(table[idx].memAddr, rReg);
            return rReg;
        case ADDSUB_ASSIGN: // TO DO: load LHS, evaluate RHS, add/sub, store back
            lhs = root->left;
            idx = findSym(lhs->lexeme);
            lReg = allocReg();
            emitMOV_RM(lReg, table[idx].memAddr);
            rReg = codeGen(root->right);
            if (hasError) { freeReg(lReg); return rReg; }
            if (root->lexeme[0] == '+') emitADD(lReg, rReg); else emitSUB(lReg, rReg);
            freeReg(rReg);
            emitMOV_MR(table[idx].memAddr, lReg);
            return lReg;
        case ADDSUB: case MULDIV: case AND: case OR: case XOR:
            lReg = codeGen(root->left);
            if (hasError) return lReg;
            rReg = codeGen(root->right);
            if (hasError) { freeReg(lReg); return rReg; }
            switch (root->data) {
                case ADDSUB:
                    if (root->lexeme[0] == '+') emitADD(lReg, rReg);
                    else emitSUB(lReg, rReg);
                    break;
                case MULDIV:
                    if (root->lexeme[0] == '*') emitMUL(lReg, rReg);
                    else emitDIV(lReg, rReg);
                    break;
                case AND:  emitAND(lReg, rReg); break;
                case OR:   emitOR (lReg, rReg); break;
                case XOR:  emitXOR(lReg, rReg); break;
                default: break;
            }
            freeReg(rReg);
            return lReg;
        default:
            return 0;
    }
}


/*============================================================================================
  TO DO: statement — compile mode (no interactive prompt; accumulate output, EXIT on ENDFILE)
============================================================================================*/

// statement := ENDFILE | END | assign_expr END
void statement(void) {
    BTNode *retp = NULL;
    int resultReg;
    if (match(ENDFILE)) {
        // TO DO: on EOF, load x/y/z into r0/r1/r2 and emit EXIT 0
        memset(regUsed, 0, sizeof(regUsed));
        emitMOV_RM(0, 0);
        emitMOV_RM(1, 4);
        emitMOV_RM(2, 8);
        emit("EXIT 0\n");
        printf("%s", outBuf);
        exit(0);
    }
    if (match(END)) {
        advance();
        return;
    }
    memset(regUsed, 0, sizeof(regUsed));
    retp = assign_expr(); // TO DO: use assign_expr instead of expr
    if (hasError) { freeTree(retp); printf("EXIT 1\n"); exit(0); }
    if (!match(END) && !match(ENDFILE)) {
        freeTree(retp); drainLine(); printf("EXIT 1\n"); exit(0);
    }
    if (match(END)) advance();
    // TO DO: validate variable usage and division-by-zero before generating code
    if (validateVars(retp)) { freeTree(retp); printf("EXIT 1\n"); exit(0); }
    if (validateDiv(retp))  { freeTree(retp); printf("EXIT 1\n"); exit(0); }
    memset(regUsed, 0, sizeof(regUsed));
    resultReg = codeGen(retp);
    if (hasError) { freeTree(retp); printf("EXIT 1\n"); exit(0); }
    freeReg(resultReg);
    freeTree(retp);
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