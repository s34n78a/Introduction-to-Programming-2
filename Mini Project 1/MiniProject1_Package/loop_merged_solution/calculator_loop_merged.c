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
    int val;
    char name[MAXLEN];
    // TO DO: add more fields if needed, such as defined/undefined, memory address, etc.
    int memAddr;    /* memory address (-1 = not in memory yet) */
    int defined;  /* 1 = has been assigned at least once (or is x/y/z) */
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

// Initialize the symbol table with builtin variables
void initTable(void);
// Get the value of a variable
int getval(char *str);
// Set the value of a variable
int setval(char *str, int val);
// Make a new node according to token type and lexeme
BTNode *makeNode(TokenSet tok, const char *lexe);
// Free the syntax tree
void freeTree(BTNode *root);
BTNode *factor(void);
// BTNode *term(void); // changed with muldiv_expr
// BTNode *term_tail(BTNode *left);
// BTNode *expr(void); // changed with addsub_expr
// BTNode *expr_tail(BTNode *left);
void statement(void);
// Print error message and exit the program
void err(ErrorType errorNum);


// for codeGen
// Evaluate the syntax tree
int evaluateTree(BTNode *root);
// Print the syntax tree in prefix
void printPrefix(BTNode *root);


/*============================================================================================
  TO DO: Definitions
============================================================================================*/

// add variables for memory management, register allocation, etc.
#define REGCOUNT 8   /* r0..r7; r0,r1,r2 reserved for x,y,z at EXIT */
/* Register allocator: simple "next free register" (never spill in this impl) */
/* r0,r1,r2 are the "home" of x,y,z but we use temps freely;
   we just pick the lowest free register each time. */
int regUsed[REGCOUNT];   /* 0 = free */

/* Next available memory address for new variables (x=0,y=4,z=8, new starts at 12) */
int nextMem = 12;

/* Output buffer so we can prepend MOV r0,[0] etc. at the very top once */
#define OUTBUF 131072
char outBuf[OUTBUF];
int  outPos = 0;

// TO DO: update parser function declarations to match new precedence hierarchy
BTNode *unary_expr(void);
BTNode *muldiv_expr(void);    /* replaces term() */
BTNode *addsub_expr(void);    /* replaces expr() */
BTNode *and_expr(void);
BTNode *xor_expr(void);
BTNode *or_expr(void);
BTNode *finish_from_id(BTNode *base);
BTNode *assign_expr(void);    /* top-level expression, replaces expr() in statement */

// TO DO: replace evaluateTree/printPrefix with codeGen (compile mode)
int codeGen(BTNode *root);


/*============================================================================================
  TO DO: Output helpers
============================================================================================*/

void emit(const char *fmt, ...) {
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

void emitMOV_RC(int d,int c) { // TO DO: move constant c to register d
    emit("MOV r%d %d\n",d,c);
}

void emitMOV_RM(int d,int a) { // TO DO: move from memory address a to register d
    emit("MOV r%d [%d]\n",d,a);
}

void emitMOV_MR(int a,int s) { // TO DO: move from register s to memory address a
    emit("MOV [%d] r%d\n",a,s);
}

void emitADD(int d,int s) { // TO DO: add register s to register d, result in d
    emit("ADD r%d r%d\n",d,s);
}

void emitSUB(int d,int s) { // TO DO: subtract register s from register d, result in d
    emit("SUB r%d r%d\n",d,s);
}

void emitMUL(int d,int s) { // TO DO: multiply register d by register s, result in d
    emit("MUL r%d r%d\n",d,s);
}

void emitDIV(int d,int s) { // TO DO: divide register d by register s, result in d
    emit("DIV r%d r%d\n",d,s);
}

void emitAND(int d,int s) { // TO DO: bitwise AND register d with register s, result in d
    emit("AND r%d r%d\n",d,s);
}

void emitOR (int d,int s) { // TO DO: bitwise OR register d with register s, result in d 
    emit("OR r%d r%d\n",d,s);
}

void emitXOR(int d,int s) { // TO DO: bitwise XOR register d with register s, result in d
    emit("XOR r%d r%d\n",d,s);}


/*============================================================================================
  TO DO: Register allocator
============================================================================================*/
int allocReg(void) {
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

void freeReg(int r) {
    if (r >= 0 && r < REGCOUNT)
        regUsed[r] = 0;
}
/* "Borrow" a register without marking it used */
/* Not needed — allocReg/freeReg suffices */


/*============================================================================================
  TO DO: Symbol table
============================================================================================*/
// for parser iniTable, getval (jadi findSym), setval (jadi addSym), makeNode, freeTree
int findSym(const char *n){ /* Return index in table, or -1 if not found.*/
    for (int i = 0; i < sbcount; i++) {
        if (strcmp(table[i].name, n) == 0) {
            return i;
        }
    }
    return -1;
}

int addSym(const char *n){
    if (sbcount >= TBLSIZE) {
        error(RUNOUT);
        return -1;
    }
    strcpy(table[sbcount].name,n); 
    table[sbcount].memAddr=nextMem;
    table[sbcount].defined=0; 
    nextMem+=4; 
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
    } else if (c == '+' || c == '-') {
        char nx = (char)fgetc(stdin); // TO DO: handle ++, --, += and -= (lookahead for '=')
        if (nx == c) { // ++ or --
            lexeme[0] = c; 
            lexeme[1] = c; 
            lexeme[2] = '\0';
            return INCDEC;
        }
        if (nx == '=') { // += or -=
            lexeme[0] = c; 
            lexeme[1] = '='; 
            lexeme[2] = '\0';
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
    } 
    
    // TO DO: add bitwise tokens and fix isalpha to support multichar and underscore (loop mirip isdigit)
    else if (isalpha(c) || c == '_') 
    {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while ((isalnum(c) || c == '_' )&& i < MAXLEN) { // loop mirip isdigit but with isalnum and underscore
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        // lexeme[1] = '\0';
        
        return ID;
    } 
    else if (c == '&')
    {
        strcpy(lexeme, "&");
        return AND;
    }
    else if (c == '|')
    {
        strcpy(lexeme, "|");
        return OR;
    }
    else if (c == '^')
    {
        strcpy(lexeme, "^");
        return XOR;
    } 
    
    else if (c == EOF) {
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
    table[0].memAddr = 0; // TO DO: assign memory address for x
    table[0].defined = 1; // Mark x as defined since it's a builtin variable
    strcpy(table[1].name, "y");
    table[1].val = 0;
    table[1].memAddr = 4; // TO DO: assign memory address for y
    table[1].defined = 1; // Mark y as defined since it's a builtin variable
    strcpy(table[2].name, "z");
    table[2].val = 0;
    table[2].memAddr = 8; // TO DO: assign memory address for z
    table[2].defined = 1; // Mark z as defined since it's a builtin variable
    sbcount = 3;
    nextMem = 12; // TO DO: set nextMem to the next available memory address for new variables
}
/* // Replaced by findSym and addSym
int getval(char *str) {
    int i = 0;

    for (i = 0; i < sbcount; i++)
        if (strcmp(str, table[i].name) == 0)
            return table[i].val;

    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    
    strcpy(table[sbcount].name, str);
    table[sbcount].val = 0;
    sbcount++;
    return 0;
}

int setval(char *str, int val) {
    int i = 0;

    for (i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0) {
            table[i].val = val;
            return val;
        }
    }

    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    
    strcpy(table[sbcount].name, str);
    table[sbcount].val = val;
    sbcount++;
    return val;
}
*/
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

// factor := INT | ADDSUB INT |
//		   	 ID  | ADDSUB ID  | 
//		   	 ID ASSIGN expr |
//		   	 LPAREN expr RPAREN |
//		   	 ADDSUB LPAREN expr RPAREN
BTNode *factor(void) {
    BTNode *retp = NULL, *left = NULL;

    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
    } else if (match(ID)) {
        left = makeNode(ID, getLexeme());
        advance();
        if (!match(ASSIGN)) {
            retp = left;
        } else {
            retp = makeNode(ASSIGN, getLexeme());
            advance();
            retp->left = left;
            retp->right = expr();
        }
    } else if (match(ADDSUB)) {
        retp = makeNode(ADDSUB, getLexeme());
        retp->left = makeNode(INT, "0");
        advance();
        if (match(INT)) {
            retp->right = makeNode(INT, getLexeme());
            advance();
        } else if (match(ID)) {
            retp->right = makeNode(ID, getLexeme());
            advance();
        } else if (match(LPAREN)) {
            advance();
            retp->right = expr();
            if (match(RPAREN))
                advance();
            else
                error(MISPAREN);
        } else {
            error(NOTNUMID);
        }
    } else if (match(LPAREN)) {
        advance();
        retp = expr();
        if (match(RPAREN))
            advance();
        else
            error(MISPAREN);
    } else {
        error(NOTNUMID);
    }
    return retp;
}

// term      := factor term_tail
// term_tail := MULDIV factor term_tail | NiL
BTNode *term(void) {
    BTNode *retp = NULL, *left = NULL;

    retp = left = factor();
    while (match(MULDIV)) {
        retp = makeNode(MULDIV, getLexeme());
        advance();
        retp->left = left;
        retp->right = factor();
        left = retp;
    }
    return retp;
}

// expr      := term expr_tail
// expr_tail := ADDSUB term expr_tail | NiL
BTNode *expr(void) {
    BTNode *retp = NULL, *left = NULL;

    retp = left = term();
    while (match(ADDSUB)) {
        retp = makeNode(ADDSUB, getLexeme());
        advance();
        retp->left = left;
        retp->right = term();
        left = retp;
    }
    return retp;
}

/* Recursion Version
BTNode *term(void) {
    BTNode *node = factor();
    return term_tail(node);
}

// term_tail := MULDIV factor term_tail | NiL
BTNode *term_tail(BTNode *left) {
    BTNode *node = NULL;

    if (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme());
        advance();
        node->left = left;
        node->right = factor();
        return term_tail(node);
    } else {
        return left;
    }
}

// expr := term expr_tail
BTNode *expr(void) {
    BTNode *node = term();
    return expr_tail(node);
}

// expr_tail := ADDSUB term expr_tail | NiL
BTNode *expr_tail(BTNode *left) {
    BTNode *node = NULL;

    if (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme());
        advance();
        node->left = left;
        node->right = term();
        return expr_tail(node);
    } else {
        return left;
    }
}
*/

// statement := ENDFILE | END | expr END
void statement(void) {
    BTNode *retp = NULL;

    if (match(ENDFILE)) {
        exit(0);
    } else if (match(END)) {
        printf(">> ");
        advance();
    } else {
        retp = expr();
        if (match(END)) {
            printf("%d\n", evaluateTree(retp));
            printf("Prefix traversal: ");
            printPrefix(retp);
            printf("\n");
            freeTree(retp);
            printf(">> ");
            advance();
        } else {
            error(SYNTAXERR);
        }
    }
}

void err(ErrorType errorNum) {
    if (PRINTERR) {
        fprintf(stderr, "error: ");
        switch (errorNum) {
            case MISPAREN:
                fprintf(stderr, "mismatched parenthesis\n");
                break;
            case NOTNUMID:
                fprintf(stderr, "number or identifier expected\n");
                break;
            case NOTFOUND:
                fprintf(stderr, "variable not defined\n");
                break;
            case RUNOUT:
                fprintf(stderr, "out of memory\n");
                break;
            case NOTLVAL:
                fprintf(stderr, "lvalue required as an operand\n");
                break;
            case DIVZERO:
                fprintf(stderr, "divide by constant zero\n");
                break;
            case SYNTAXERR:
                fprintf(stderr, "syntax error\n");
                break;
            default:
                fprintf(stderr, "undefined error\n");
                break;
        }
    }
    printf("EXIT 1\n"); // TO DO: print EXIT 1 on error before exiting
    exit(0);
}


/*============================================================================================
codeGen implementation
============================================================================================*/
/* // Replaced by codeGen
int evaluateTree(BTNode *root) {
    int retval = 0, lv = 0, rv = 0;

    if (root != NULL) {
        switch (root->data) {
            case ID:
                retval = getval(root->lexeme);
                break;
            case INT:
                retval = atoi(root->lexeme);
                break;
            case ASSIGN:
                rv = evaluateTree(root->right);
                retval = setval(root->left->lexeme, rv);
                break;
            case ADDSUB:
            case MULDIV:
                lv = evaluateTree(root->left);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+") == 0) {
                    retval = lv + rv;
                } else if (strcmp(root->lexeme, "-") == 0) {
                    retval = lv - rv;
                } else if (strcmp(root->lexeme, "*") == 0) {
                    retval = lv * rv;
                } else if (strcmp(root->lexeme, "/") == 0) {
                    if (rv == 0)
                        error(DIVZERO);
                    retval = lv / rv;
                }
                break;
            default:
                retval = 0;
        }
    }
    return retval;
}
*/
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

int main() {
    initTable();
    printf(">> ");
    while (1) {
        statement();
    }
    return 0;
}