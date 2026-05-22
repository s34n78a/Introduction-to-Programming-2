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
int setval(char *str /*, int val*/); // TO DO: setval no longer takes val param since it just adds an undefined variable to the table and returns its index
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


/*============================================================================================
  TO DO: Output helpers
============================================================================================*/

void emitMOV_RC(int d,int c) { // TO DO: move constant c to register d
    outPos += sprintf(outBuf+outPos, "MOV r%d %d\n", d, c);
}

void emitMOV_RM(int d,int a) { // TO DO: move from memory address a to register d
    outPos += sprintf(outBuf+outPos, "MOV r%d [%d]\n", d, a);
}

void emitMOV_MR(int a,int s) { // TO DO: move from register s to memory address a
    outPos += sprintf(outBuf+outPos, "MOV [%d] r%d\n", a,s);
}

void emitADD(int d,int s) { // TO DO: add register s to register d, result in d
    outPos += sprintf(outBuf+outPos, "ADD r%d r%d\n", d ,s);
}

void emitSUB(int d,int s) { // TO DO: subtract register s from register d, result in d
    outPos += sprintf(outBuf+outPos, "SUB r%d r%d\n", d ,s);
}

void emitMUL(int d,int s) { // TO DO: multiply register d by register s, result in d
    outPos += sprintf(outBuf+outPos, "MUL r%d r%d\n", d ,s);
}

void emitDIV(int d,int s) { // TO DO: divide register d by register s, result in d
    outPos += sprintf(outBuf+outPos, "DIV r%d r%d\n", d ,s);
}

void emitAND(int d,int s) { // TO DO: bitwise AND register d with register s, result in d
    outPos += sprintf(outBuf+outPos, "AND r%d r%d\n", d ,s);
}

void emitOR (int d,int s) { // TO DO: bitwise OR register d with register s, result in d 
    outPos += sprintf(outBuf+outPos, "OR r%d r%d\n", d ,s);
}

void emitXOR(int d,int s) { // TO DO: bitwise XOR register d with register s, result in d
    outPos += sprintf(outBuf+outPos, "XOR r%d r%d\n", d ,s);}

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
  TO DO: Variable validation and constant folding helpers
============================================================================================*/

int hasVariable(BTNode *r) { // returns 1 if the subtree contains any ID node
    if (!r) return 0;
    if (r->data == ID) return 1;
    return hasVariable(r->left) || hasVariable(r->right);
}

int constEval(BTNode *r, int *res) { // statically evaluates a constant-only subtree then returns 1 and sets *res on success, 0 if tree has variables or hits div-by-zero
    int lv, rv;
    if (!r)
    { 
        *res = 0; 
        return 1; 
    }
    if (r->data == INT) 
    { 
        *res = atoi(r->lexeme); 
        return 1; 
    }
    if (r->data == ID || r->data == INCDEC) 
    {
        return 0;
    }
    if (!constEval(r->left,  &lv)) 
    {
        return 0;
    }
    if (!constEval(r->right, &rv)) 
    {
        return 0;
    }
    switch (r->data) {
        case ADDSUB: 
            if (r->lexeme[0] == '+')
            {
                *res = lv + rv;
            }
            else
            {
                *res = lv - rv;
            }
            return 1;
        case MULDIV:
            if (r->lexeme[0] == '*') 
            { 
                *res = lv * rv; 
                return 1; 
            }
            if (rv == 0)
            {
                return 0; // div by zero
            }
            *res = lv / rv; 
            return 1;
        case AND: 
            *res = lv & rv; 
            return 1;
        case OR:  
            *res = lv | rv; 
            return 1;
        case XOR: 
            *res = lv ^ rv; 
            return 1;
        default: 
            return 0;
    }
}

int validateDiv(BTNode *r) {// returns 1 if any division by constant zero is found
    int val;
    if (!r) 
    {
        return 0;
    }
    if (r->data == MULDIV && r->lexeme[0] == '/') 
    {
        if (!hasVariable(r->right)) /* If RHS has no variable, constant-eval it */
        {
            if (!constEval(r->right, &val) || val == 0) /* constEval failed: means nested div by zero in RHS */
            {
                return 1;
            }
        }
    }
    return validateDiv(r->left) || validateDiv(r->right);
}

int validateVars(BTNode *r) {// returns 1 if any variable is used before being defined
    int idx;
    if (!r)
    {
        return 0;
    }
    if (r->data == ASSIGN) {
        BTNode *lhs = r->left;
        if (!lhs || lhs->data != ID) // the left side of an assignment must be an ID
        {
            return 1;
        }
        idx = getval(lhs->lexeme);
        if (idx < 0) // new variable on LHS — add to symbol table as undefined (will be marked defined in validateVars pass)
        {
            idx = setval(lhs->lexeme);
        }
        if (idx < 0) /* setval failed (e.g. out of memory) */
        {
            return 1;
        }
        
        table[idx].defined = 1;
        return validateVars(r->right);
    }
    if (r->data == ADDSUB_ASSIGN) {
        BTNode *lhs = r->left;
        if (!lhs || lhs->data != ID)
        {
            return 1;
        }
        idx = getval(lhs->lexeme);
        if (idx < 0 || !table[idx].defined)
        {
            return 1;
        }
        return validateVars(r->right);
    }
    if (r->data == ID) {
        idx = getval(r->lexeme);
        if (idx < 0 || !table[idx].defined)
        {
            return 1;
        }
        return 0;
    }
    if (r->data == INCDEC) {
        BTNode *id = r->left;
        if (!id || id->data != ID)
        {
            return 1;
        }
        idx = getval(id->lexeme);
        if (idx < 0 || !table[idx].defined)
        {
            return 1;
        }
        return 0;
    }
    return validateVars(r->left) || validateVars(r->right);
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

int getval(char *str) {
    int i = 0;

    for (i = 0; i < sbcount; i++)
        if (strcmp(str, table[i].name) == 0) {
            return i; // TO DO: returns table index of str, or -1 if not found ---> return table[i].val;
        }
    return -1; /* Return index in table, or -1 if not found.*/

    /* if (sbcount >= TBLSIZE)
        error(RUNOUT);
    
    strcpy(table[sbcount].name, str);
    table[sbcount].val = 0;
    sbcount++;
    return 0;*/
}

int setval(char *str /*, int val*/) {
    /*int i = 0;

    for (i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0) {
            table[i].val = val;
            return val;
        }
    }*/

    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    
    strcpy(table[sbcount].name, str);
    // table[sbcount].val = val;
    table[sbcount].memAddr = nextMem; // TO DO: assign memory address for new variable
    table[sbcount].defined = 0; // Mark new variable as undefined until it's assigned in validateVars
    nextMem += 4; // TO DO: increment nextMem for the next variable
    return sbcount++; // TO DO: return index in table instead of value
    
    /* sbcount++;
    return val;*/
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

// factor := INT | ADDSUB INT | ID  | ADDSUB ID | ID ASSIGN expr | LPAREN expr RPAREN | ADDSUB LPAREN expr RPAREN
BTNode *factor(void) {
    BTNode *retp = NULL /*, *left = NULL*/;

    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
        return retp; // TO DO: early return for simple cases like INT and ID to avoid unnecessary nesting
    } else if (match(ID)) {
        retp /*left*/ = makeNode(ID, getLexeme());
        advance();
        /* if (!match(ASSIGN)) { retp = left; } 
        else { retp = makeNode(ASSIGN, getLexeme());
            advance(); retp->left = left; retp->right = expr();}*/
        return retp;// TO DO: handle ID ASSIGN expr (e.g. x = 3 or y = x + 2)
    } 
    /*else if (match(ADDSUB)) {
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
    } */
    // TO DO: handle INCDEC ID (e.g. ++x or --x) and parenthesized expressions (LPAREN expr RPAREN), also with optional leading ADDSUB for unary plus/minus
    else if (match(INCDEC)) {
        char op[MAXLEN]; 
        strcpy(op, getLexeme()); 
        advance();
        if (!match(ID)) 
        { 
            error(NOTNUMID); 
        }
        retp = makeNode(INCDEC, op);
        retp->left = makeNode(ID, getLexeme());
        advance();
        return retp;
    }

    else if (match(LPAREN)) {
        advance();
        retp = assign_expr(); // TODO: recurse into assign_expr instead of expr ---> retp = expr();
        if (match(RPAREN))
            advance();
        else
            error(MISPAREN);
        return retp; // TO DO: early return for parenthesized expr to avoid unnecessary nesting
    } else {
        error(NOTNUMID);
        return makeNode(INT, "0"); // TO DO: Return a dummy node to avoid null pointer issues in the caller; this node will be ignored since hasError is set
    }
    // return retp;
}

/* term      := factor term_tail
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
} // Replace with full grammar */


/*============================================================================================
  TO DO: Full grammar implementation
============================================================================================*/
//       New call chain: assign_expr -> or_expr -> xor_expr -> and_expr -> addsub_expr -> muldiv_expr -> unary_expr -> factor

// unary_expr := ADDSUB unary_expr | factor
BTNode *unary_expr(void) { // TODO: unary_expr — handles unary +/- as prefix (builds 0 - x style node)
    BTNode *node;
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
// muldiv_expr := unary_expr muldiv_expr_tail
// muldiv_expr_tail := MULDIV unary_expr muldiv_expr_tail | NiL
BTNode *muldiv_expr(void) { // TODO: muldiv_expr — same structure as old term(), but calls unary_expr
    BTNode *left, *node;
    left = unary_expr();
    while (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme()); 
        advance();
        node->left = left; 
        node->right = unary_expr(); 
        left = node;
    }
    return left;
}

// addsub_expr := muldiv_expr addsub_expr_tail
// addsub_expr_tail := ADDSUB muldiv_expr addsub_expr_tail | NiL
BTNode *addsub_expr(void) { // TODO: addsub_expr — same structure as old expr(), but calls muldiv_expr
    BTNode *left, *node;
    left = muldiv_expr();
    while (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme()); 
        advance();
        node->left = left; 
        node->right = muldiv_expr(); 
        left = node;
    }
    return left;
}

// and_expr := addsub_expr and_expr_tail
// and_expr_tail := AND addsub_expr and_expr_tail | NiL
BTNode *and_expr(void) { // TODO: and_expr — new precedence level for &
    BTNode *left, *node;
    left = addsub_expr();
    while (match(AND)) {
        node = makeNode(AND, "&"); 
        advance();
        node->left = left; 
        node->right = addsub_expr(); 
        left = node;
    }
    return left;
}

// xor_expr := and_expr xor_expr_tail
// xor_expr_tail := XOR and_expr xor_expr_tail | NiL
BTNode *xor_expr(void) { // TODO: xor_expr — new precedence level for ^
    BTNode *left, *node;
    left = and_expr();
    while (match(XOR)) {
        node = makeNode(XOR, "^"); 
        advance();
        node->left = left; 
        node->right = and_expr(); 
        left = node;
    }
    return left;
}

// or_expr := xor_expr or_expr_tail
// or_expr_tail := OR xor_expr or_expr_tail | NiL
BTNode *or_expr(void) { // TODO: or_expr — new precedence level for |
    BTNode *left, *node;
    left = xor_expr();
    while (match(OR)) {
        node = makeNode(OR, "|"); 
        advance();
        node->left = left; 
        node->right = xor_expr(); 
        left = node;
    }
    return left;
}

//       consumed an ID and found it is NOT followed by = or +=/-=
BTNode *finish_from_id(BTNode *base) { // TODO: finish_from_id — re-enters the full binary-op chain when we have already
    BTNode *left = base, *node;
    while (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme()); 
        advance();
        node->left = left; 
        node->right = unary_expr(); 
        left = node;
    }
    while (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme()); 
        advance();
        node->left = left; 
        node->right = muldiv_expr(); 
        left = node;
    }
    while (match(AND)) {
        node = makeNode(AND, "&"); 
        advance();
        node->left = left; 
        node->right = addsub_expr(); 
        left = node;
    }
    while (match(XOR)) {
        node = makeNode(XOR, "^"); 
        advance();
        node->left = left; 
        node->right = and_expr(); 
        left = node;
    }
    while (match(OR)) {
        node = makeNode(OR, "|"); 
        advance();
        node->left = left; 
        node->right = xor_expr(); 
        left = node;
    }
    return left;
}

// assign_expr := ID ASSIGN assign_expr | ID ADDSUB_ASSIGN assign_expr | or_expr
BTNode *assign_expr(void) { // TODO: assign_expr — top-level rule; peek past ID to detect = or +=/-=
    BTNode *node, *lhs;
    char savedLex[MAXLEN], oplex[MAXLEN];
    if (match(ID)) { // We need look-ahead: if current token is ID and next is ASSIGN/ADDSUB_ASSIGN we take the assignment branch; otherwise fall through to or_expr.
        strcpy(savedLex, getLexeme()); 
        advance(); // Because our lexer is one-token, we peek by remembering that match(ID) is true and we already have the lexeme. We call advance() to get the next token and check it.
        if (match(ASSIGN)) {
            lhs = makeNode(ID, savedLex); 
            node = makeNode(ASSIGN, "=");
            advance(); 
            node->left = lhs; 
            node->right = assign_expr(); 
            return node;
        }
        if (match(ADDSUB_ASSIGN)) {
            strcpy(oplex, getLexeme());
            lhs = makeNode(ID, savedLex); 
            node = makeNode(ADDSUB_ASSIGN, oplex);
            advance(); node->left = lhs; 
            node->right = assign_expr(); 
            return node;
        }
        return finish_from_id(makeNode(ID, savedLex));
    }
    return or_expr();
}

// statement := ENDFILE | END | expr END
void statement(void) {
    BTNode *retp = NULL;
    int resultReg; // TO DO: register to hold the result of evaluating the expression for printing
    if (match(ENDFILE)) {
        memset(regUsed, 0, sizeof(regUsed)); // TODO: store final x,y,z into r0,r1,r2 then print buffered output
        emitMOV_RM(0, 0); 
        emitMOV_RM(1, 4); 
        emitMOV_RM(2, 8);
        outPos += sprintf(outBuf+outPos, "EXIT 0\n");
        printf("%s", outBuf);

        exit(0);
    } else if (match(END)) {
        // printf(">> "); // TO DO: delete this line since we will print the prompt in main loop instead of here, to avoid printing an extra prompt after the last statement
        advance();
    } else {
        memset(regUsed, 0, sizeof(regUsed)); // TODO: reset register allocator for each new statement
        retp = assign_expr(); // TODO: call assign_expr instead of expr ---> retp = expr();

        if (match(END)) {
            /*printf("%d\n", evaluateTree(retp));
            printf("Prefix traversal: ");
            printPrefix(retp);
            printf("\n");*/

            // TODO: validate then generate code instead of evaluating
            if (validateVars(retp)) 
            {
                err(NOTFOUND);
            }
            if (validateDiv(retp))  
            {
                err(DIVZERO);
            }
            memset(regUsed, 0, sizeof(regUsed));
            resultReg = evaluateTree(retp);
            freeReg(resultReg);

            freeTree(retp);
            // printf(">> "); // TO DO: delete this line since we will print the prompt in main loop instead of here, to avoid printing an extra prompt after the last statement
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

int evaluateTree(BTNode *root) {
    int retval = 0, lv = 0, rv = 0;

    if (root != NULL) {
        switch (root->data) {
            case ID:
                retval = allocReg(); // TODO: Replace retval ---> retval = getval(root->lexeme);with the register number
                emitMOV_RM(retval, table[getval(root->lexeme)].memAddr); // TO DO: emit MOV to load into register
                
                break;
            case INT:
                retval = allocReg(); // TO DO: Replace retval --> retval = atoi(root->lexeme); with the register number 
                emitMOV_RC(retval, atoi(root->lexeme)); // TO DO: emit MOV to load the integer constant into the register

                break;
            
            case INCDEC: // TODO: INCDEC — load variable, add/sub 1, write back, return updated reg
                retval = allocReg();
                emitMOV_RM(retval, table[getval(root->left->lexeme)].memAddr);
                rv = allocReg();
                emitMOV_RC(rv, 1);
                if (root->lexeme[0] == '+') 
                {
                    emitADD(retval, rv);
                }
                else                        
                {
                    emitSUB(retval, rv);
                }
                freeReg(rv);
                emitMOV_MR(table[getval(root->left->lexeme)].memAddr, retval);
                break;

            case ASSIGN:
                rv = evaluateTree(root->right);
                
                emitMOV_MR(table[getval(root->left->lexeme)].memAddr, rv); // TODO: generate RHS, store result into LHS memory address and replace --> retval = setval(root->left->lexeme, rv);
                retval = rv;  // return rv so chained assigns (x = y = 3) work
                
                break;
            
            case ADDSUB_ASSIGN: // TODO: ADDSUB_ASSIGN similar to INCDEC — load LHS, compute RHS, add/sub, write back
                lv = allocReg();
                emitMOV_RM(lv, table[getval(root->left->lexeme)].memAddr);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+=") == 0) 
                {
                    emitADD(lv, rv);
                }
                else                        
                {
                    emitSUB(lv, rv);
                }
                freeReg(rv);
                emitMOV_MR(table[getval(root->left->lexeme)].memAddr, lv);
                retval = lv;
                break;

            case ADDSUB:
            case MULDIV:
            
            case AND:// TODO: add AND, OR, XOR — same structure as ADDSUB/MULDIV
            case OR:
            case XOR:
                lv = evaluateTree(root->left);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+") == 0) {
                    emitADD(lv, rv); // TO DO: emit instead of compute ---> retval = lv + rv;
                } else if (strcmp(root->lexeme, "-") == 0) {
                    emitSUB(lv, rv); // TO DO: emit instead of compute ---> retval = lv - rv;
                } else if (strcmp(root->lexeme, "*") == 0) {
                    emitMUL(lv, rv); // TO DO: emit instead of compute ---> retval = lv * rv;
                } else if (strcmp(root->lexeme, "/") == 0) {
                    emitDIV(lv, rv); // TO DO: emit instead of compute --->
                    /*if (rv == 0)
                        error(DIVZERO);
                    retval = lv / rv;*/
                } 

                // TO DO: handle AND, OR, XOR with emit and set retval to the correct register
                else if (strcmp(root->lexeme, "&") == 0) {
                    emitAND(lv, rv);
                } else if (strcmp(root->lexeme, "|") == 0) {
                    emitOR(lv, rv);
                } else if (strcmp(root->lexeme, "^") == 0) {
                    emitXOR(lv, rv);
                }
                freeReg(rv);  // free RHS register, result lives in lv
                retval = lv; // result is now in lv, so set retval to lv to return the correct register number

                break;
            default:
                retval = 0;
        }
    }
    return retval;
}

/*void printPrefix(BTNode *root) {
    if (root != NULL) {
        printf("%s ", root->lexeme);
        printPrefix(root->left);
        printPrefix(root->right);
    }
}*/

/*============================================================================================
main
============================================================================================*/

int main() {
    initTable();
    // printf(">> "); // TO DO: remove this line since we will print the prompt in the main loop instead of in statement()
    while (1) {
        statement();
    }
    return 0;
}