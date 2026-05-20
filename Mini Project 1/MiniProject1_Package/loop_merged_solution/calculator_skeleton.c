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
    // TO DO: format the string with va_list and append it to outBuf
}
static void emitMOV_RR(int d, int s) { // TO DO: move from register s to register d
}
static void emitMOV_RC(int d, int c) { // TO DO: move constant c to register d
}
static void emitMOV_RM(int d, int a) { // TO DO: move from memory address a to register d
}
static void emitMOV_MR(int a, int s) { // TO DO: move from register s to memory address a
}
static void emitADD(int d, int s) { // TO DO: add register s to register d, result in d
}
static void emitSUB(int d, int s) { // TO DO: subtract register s from register d, result in d
}
static void emitMUL(int d, int s) { // TO DO: multiply register d by register s, result in d
}
static void emitDIV(int d, int s) { // TO DO: divide register d by register s, result in d
}
static void emitAND(int d, int s) { // TO DO: bitwise AND register d with register s, result in d
}
static void emitOR (int d, int s) { // TO DO: bitwise OR register d with register s, result in d
}
static void emitXOR(int d, int s) { // TO DO: bitwise XOR register d with register s, result in d
}
/*============================================================================================
  TO DO: Register allocator
============================================================================================*/
static int allocReg(void) {
    // TO DO: find and return the index of the first free register; exit if none available
    return 0;
}
static void freeReg(int r) {
    // TO DO: mark register r as free
}
/*============================================================================================
  TO DO: Symbol table
============================================================================================*/
// TO DO: getval/setval replaced by findSym/addSym only (no value tracking in compile mode)
static int findSym(const char *n) { /* Return index in table, or -1 if not found */
    // TO DO: search table[0..sbcount-1] by name; return index or -1
    return -1;
}
static int addSym(const char *n) {
    // TO DO: add a new symbol with nextMem address; advance nextMem by 4; return index or -1
    return -1;
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
    if (isdigit((unsigned char)c)) {
        // TO DO: read all digit characters into lexeme and return INT
    } else if (isalpha((unsigned char)c) || c == '_') { // TO DO: support multi-char identifiers and underscore
        // TO DO: read all alphanumeric / underscore characters into lexeme and return ID
    } else if (c == '+' || c == '-') {
        // TO DO: detect ++ / -- (INCDEC) and += / -= (ADDSUB_ASSIGN)
        // otherwise return ADDSUB
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
    else {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return UNKNOWN;
    }
    return UNKNOWN;
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
    // TO DO: advance until END or ENDFILE, then consume END
}
void err(ErrorType errorNum) {
    // TO DO: drain the current line and set hasError = 1 (no exit here)
}
// factor := INT | ID | INCDEC ID | LPAREN assign_expr RPAREN
// TO DO: factor no longer handles ADDSUB (moved to unary_expr) or ASSIGN (moved to assign_expr)
BTNode *factor(void) {
    // TO DO: implement factor
    return NULL;
}
// TO DO: unary_expr handles unary ADDSUB (was mixed into factor before)
// unary_expr := ADDSUB unary_expr | factor
BTNode *unary_expr(void) {
    // TO DO: implement unary_expr
    return NULL;
}
// muldiv_expr := unary_expr (MULDIV unary_expr)*
// TO DO: replaces term(); now calls unary_expr instead of factor()
BTNode *muldiv_expr(void) {
    // TO DO: implement muldiv_expr
    return NULL;
}
// addsub_expr := muldiv_expr (ADDSUB muldiv_expr)*
// TO DO: replaces expr(); now calls muldiv_expr instead of term()
BTNode *addsub_expr(void) {
    // TO DO: implement addsub_expr
    return NULL;
}
// TO DO: new precedence levels for bitwise operators (AND < XOR < OR, all below ADDSUB)
// and_expr := addsub_expr (AND addsub_expr)*
BTNode *and_expr(void) {
    // TO DO: implement and_expr
    return NULL;
}
// xor_expr := and_expr (XOR and_expr)*
BTNode *xor_expr(void) {
    // TO DO: implement xor_expr
    return NULL;
}
// or_expr := xor_expr (OR xor_expr)*
BTNode *or_expr(void) {
    // TO DO: implement or_expr
    return NULL;
}
// TO DO: finish_from_id continues binary operator chain after an ID has already been consumed
// Needed because assign_expr peeks at the next token after ID before deciding to assign or not
BTNode *finish_from_id(BTNode *base) {
    // TO DO: implement finish_from_id — continue MULDIV / ADDSUB / AND / XOR / OR chains
    return base;
}
// TO DO: assign_expr is the new top-level expression rule (replaces expr() in statement)
// assign_expr := ID ASSIGN assign_expr
//              | ID ADDSUB_ASSIGN assign_expr
//              | or_expr
BTNode *assign_expr(void) {
    // TO DO: implement assign_expr
    return NULL;
}
/*============================================================================================
  TO DO: Validation helpers
============================================================================================*/
// TO DO: check if a subtree contains any variable reference
static int hasVariable(BTNode *r) {
    // TO DO: return 1 if any node in the subtree has data == ID
    return 0;
}
// TO DO: try to constant-fold a subtree; returns 1 on success and writes result to *res
static int constEval(BTNode *r, int *res) {
    // TO DO: recursively evaluate a purely constant subtree; return 0 if any variable found
    return 0;
}
// TO DO: walk the tree and detect any division by a constant zero
static int validateDiv(BTNode *r) {
    // TO DO: return 1 if a MULDIV '/' node has a constant-zero right child
    return 0;
}
// TO DO: walk the tree and detect uses of undefined variables;
//        marks variables as defined when an ASSIGN or ADDSUB_ASSIGN is seen
static int validateVars(BTNode *r) {
    // TO DO: return 1 if any ID is used before being defined
    return 0;
}
/*============================================================================================
codeGen implementation
============================================================================================*/
// TO DO: codeGen replaces evaluateTree — emits assembly instructions instead of evaluating
// Returns the register number holding the result
int codeGen(BTNode *root) {
    // TO DO: implement codeGen
    //   INT          -> allocReg, emitMOV_RC
    //   ID           -> allocReg, emitMOV_RM
    //   INCDEC       -> load, add/sub 1, store, return updated register
    //   ASSIGN       -> codeGen(right), emitMOV_MR to LHS address, return rReg
    //   ADDSUB_ASSIGN-> load LHS, codeGen(right), add/sub, store, return lReg
    //   ADDSUB/MULDIV/AND/OR/XOR -> codeGen(left), codeGen(right), emit op, freeReg(rReg)
    return 0;
}
/*============================================================================================
  TO DO: statement — compile mode (no interactive prompt; accumulate output, EXIT on ENDFILE)
============================================================================================*/
// statement := ENDFILE | END | assign_expr END
void statement(void) {
    BTNode *retp = NULL;
    int resultReg;
    if (match(ENDFILE)) {
        // TO DO: on EOF, load x/y/z into r0/r1/r2 and emit EXIT 0, then print outBuf and exit
    }
    if (match(END)) {
        advance();
        return;
    }
    // TO DO: use assign_expr instead of expr
    retp = assign_expr();
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
// This package is a calculator
// It works like a Python interpretor
// Example:
// >> y = 2
// >> z = 2
// >> x = 3 * y + 4 / (2 * z)
// It will print the answer of every line
// You should turn it into an expression compiler
// And print the assembly code according to the input
// This is the grammar used in this package
// You can modify it according to the spec and the slide
// statement   := ENDFILE | END | assign_expr END
// assign_expr := ID ASSIGN assign_expr | ID ADDSUB_ASSIGN assign_expr | or_expr
// or_expr     := xor_expr (OR xor_expr)*
// xor_expr    := and_expr (XOR and_expr)*
// and_expr    := addsub_expr (AND addsub_expr)*
// addsub_expr := muldiv_expr (ADDSUB muldiv_expr)*
// muldiv_expr := unary_expr (MULDIV unary_expr)*
// unary_expr  := ADDSUB unary_expr | factor
// factor      := INT | ID | INCDEC ID |
//                LPAREN assign_expr RPAREN
int main(void) {
    initTable();
    while (1) {
        statement();
    }
    return 0;
}