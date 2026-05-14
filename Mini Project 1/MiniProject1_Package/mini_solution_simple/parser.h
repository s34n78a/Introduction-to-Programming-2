#ifndef __PARSER__
#define __PARSER__

#include "lex.h"
#define TBLSIZE 256

#define PRINTERR 1

#define error(errorNum) { \
    if (PRINTERR) \
        fprintf(stderr, "error() called at %s:%d: ", __FILE__, __LINE__); \
    err(errorNum); \
}

typedef enum {
    UNDEFINED, MISPAREN, NOTNUMID, NOTFOUND, RUNOUT, NOTLVAL, DIVZERO, SYNTAXERR, DIVZERO_CONST
} ErrorType;

typedef struct {
    char name[MAXLEN];
    int val;
    int defined;  // 1 if defined, 0 if not
    int addr;     // memory address for this variable
} Symbol;

typedef struct _Node {
    TokenSet data;
    int val;
    char lexeme[MAXLEN];
    struct _Node *left;
    struct _Node *right;
    int hasVar;  // 1 if subtree contains a variable
} BTNode;

extern Symbol table[TBLSIZE];
extern int sbcount;

extern void initTable(void);
extern int getval(char *str);
extern int setval(char *str, int val);
extern int isDefined(char *str);
extern int newVar(char *str);

extern BTNode *makeNode(TokenSet tok, const char *lexe);
extern void freeTree(BTNode *root);

extern BTNode *assign_expr(void);
extern BTNode *or_expr(void);
extern BTNode *xor_expr(void);
extern BTNode *and_expr(void);
extern BTNode *addsub_expr(void);
extern BTNode *muldiv_expr(void);
extern BTNode *unary_expr(void);
extern BTNode *factor(void);

extern void statement(void);
extern void err(ErrorType errorNum);

#endif
