#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "codeGen.h"

int sbcount = 0;
Symbol table[TBLSIZE];

void initTable(void) {
    strcpy(table[0].name, "x");
    table[0].val = 0;
    table[0].defined = 1;
    table[0].addr = 0;
    
    strcpy(table[1].name, "y");
    table[1].val = 0;
    table[1].defined = 1;
    table[1].addr = 4;
    
    strcpy(table[2].name, "z");
    table[2].val = 0;
    table[2].defined = 1;
    table[2].addr = 8;
    
    sbcount = 3;
}

int isDefined(char *str) {
    for (int i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0)
            return table[i].defined;
    }
    return 0;
}

int getval(char *str) {
    for (int i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0)
            return table[i].val;
    }
    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    strcpy(table[sbcount].name, str);
    table[sbcount].val = 0;
    table[sbcount].defined = 0;
    table[sbcount].addr = sbcount * 4;
    sbcount++;
    return 0;
}

int newVar(char *str) {
    for (int i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0)
            return i;
    }
    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    strcpy(table[sbcount].name, str);
    table[sbcount].val = 0;
    table[sbcount].defined = 1;
    table[sbcount].addr = sbcount * 4;
    sbcount++;
    return sbcount - 1;
}

int setval(char *str, int val) {
    for (int i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0) {
            table[i].val = val;
            return val;
        }
    }
    if (sbcount >= TBLSIZE)
        error(RUNOUT);
    strcpy(table[sbcount].name, str);
    table[sbcount].val = val;
    table[sbcount].defined = 1;
    table[sbcount].addr = sbcount * 4;
    sbcount++;
    return val;
}

BTNode *makeNode(TokenSet tok, const char *lexe) {
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    strcpy(node->lexeme, lexe);
    node->data = tok;
    node->val = 0;
    node->left = NULL;
    node->right = NULL;
    node->hasVar = 0;
    return node;
}

void freeTree(BTNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// factor := INT | ID | INCDEC ID | LPAREN assign_expr RPAREN
BTNode *factor(void) {
    BTNode *retp = NULL;

    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
        return retp;
    }

    if (match(ID)) {
        char idname[MAXLEN];
        strcpy(idname, getLexeme());
        retp = makeNode(ID, idname);
        retp->hasVar = 1;
        
        advance();
        return retp;
    }

    if (match(INCDEC)) {
        char op[MAXLEN];
        strcpy(op, getLexeme());
        advance();
        
        if (!match(ID))
            error(NOTNUMID);
        
        char idname[MAXLEN];
        strcpy(idname, getLexeme());
        
        if (!isDefined(idname))
            error(NOTFOUND);
        
        retp = makeNode(INCDEC, op);
        retp->left = makeNode(ID, idname);
        retp->left->hasVar = 1;
        retp->hasVar = 1;
        advance();
        return retp;
    }

    if (match(LPAREN)) {
        advance();
        retp = assign_expr();
        if (!match(RPAREN))
            error(MISPAREN);
        advance();
        return retp;
    }

    error(NOTNUMID);
    return NULL;
}

// unary_expr := ADDSUB unary_expr | factor
BTNode *unary_expr(void) {
    if (match(ADDSUB)) {
        BTNode *retp = makeNode(ADDSUB, getLexeme());
        retp->left = makeNode(INT, "0");
        advance();
        retp->right = unary_expr();
        retp->hasVar = retp->right->hasVar;
        return retp;
    }
    return factor();
}

// muldiv_expr := unary_expr muldiv_expr_tail
// muldiv_expr_tail := MULDIV unary_expr muldiv_expr_tail | NiL
BTNode *muldiv_expr(void) {
    BTNode *retp = unary_expr();
    
    while (match(MULDIV)) {
        BTNode *op = makeNode(MULDIV, getLexeme());
        advance();
        BTNode *right = unary_expr();
        
        // Check div by zero: if right is constant-only and evaluates to 0
        if (strcmp(op->lexeme, "/") == 0 && !right->hasVar) {
            // Try to evaluate right subtree at parse time
            if (evaluateConst(right) == 0) {
                error(DIVZERO_CONST);
            }
        }
        
        op->left = retp;
        op->right = right;
        op->hasVar = retp->hasVar || right->hasVar;
        retp = op;
    }
    return retp;
}

// addsub_expr := muldiv_expr addsub_expr_tail
// addsub_expr_tail := ADDSUB muldiv_expr addsub_expr_tail | NiL
BTNode *addsub_expr(void) {
    BTNode *retp = muldiv_expr();
    
    while (match(ADDSUB)) {
        BTNode *op = makeNode(ADDSUB, getLexeme());
        advance();
        BTNode *right = muldiv_expr();
        op->left = retp;
        op->right = right;
        op->hasVar = retp->hasVar || right->hasVar;
        retp = op;
    }
    return retp;
}

// and_expr := addsub_expr and_expr_tail
// and_expr_tail := AND addsub_expr and_expr_tail | NiL
BTNode *and_expr(void) {
    BTNode *retp = addsub_expr();
    
    while (match(AND)) {
        BTNode *op = makeNode(AND, getLexeme());
        advance();
        BTNode *right = addsub_expr();
        op->left = retp;
        op->right = right;
        op->hasVar = retp->hasVar || right->hasVar;
        retp = op;
    }
    return retp;
}

// xor_expr := and_expr xor_expr_tail
// xor_expr_tail := XOR and_expr xor_expr_tail | NiL
BTNode *xor_expr(void) {
    BTNode *retp = and_expr();
    
    while (match(XOR)) {
        BTNode *op = makeNode(XOR, getLexeme());
        advance();
        BTNode *right = and_expr();
        op->left = retp;
        op->right = right;
        op->hasVar = retp->hasVar || right->hasVar;
        retp = op;
    }
    return retp;
}

// or_expr := xor_expr or_expr_tail
// or_expr_tail := OR xor_expr or_expr_tail | NiL
BTNode *or_expr(void) {
    BTNode *retp = xor_expr();
    
    while (match(OR)) {
        BTNode *op = makeNode(OR, getLexeme());
        advance();
        BTNode *right = xor_expr();
        op->left = retp;
        op->right = right;
        op->hasVar = retp->hasVar || right->hasVar;
        retp = op;
    }
    return retp;
}

// assign_expr := ID ASSIGN assign_expr | ID ADDSUB_ASSIGN assign_expr | or_expr
BTNode *assign_expr(void) {
    BTNode *retp = or_expr();
    
    // Check if this is an assignment: retp should be ID node, and next token ASSIGN or ADDSUB_ASSIGN
    if (retp != NULL && retp->data == ID) {
        char idname[MAXLEN];
        strcpy(idname, retp->lexeme);
        
        if (match(ASSIGN)) {
            advance();
            BTNode *assign = makeNode(ASSIGN, "=");
            assign->left = retp;
            
            // Define variable if first appearance on LHS
            int found = 0;
            for (int i = 0; i < sbcount; i++) {
                if (strcmp(idname, table[i].name) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                newVar(idname);
            }
            
            assign->right = assign_expr();
            assign->hasVar = assign->right->hasVar || 1;
            return assign;
        } else if (match(ADDSUB_ASSIGN)) {
            char op[MAXLEN];
            strcpy(op, getLexeme());
            advance();

            if (!isDefined(idname))
                error(NOTFOUND);
            BTNode *assignNode = makeNode(ADDSUB_ASSIGN, op);
            assignNode->left = retp;
            assignNode->right = assign_expr();
            assignNode->hasVar = assignNode->right->hasVar || 1;
            return assignNode;
        }
    }
    
    // Not an assignment; check if ID is defined when it's used
    if (retp != NULL && retp->data == ID) {
        if (!isDefined(retp->lexeme))
            error(NOTFOUND);
    }
    
    return retp;
}

// statement := ENDFILE | END | assign_expr END
void statement(void) {
    if (match(ENDFILE)) {
        exit(0);
    } else if (match(END)) {
        advance();
    } else {
        BTNode *retp = assign_expr();
        if (!match(END))
            error(SYNTAXERR);
        
        generateCode(retp);
        freeTree(retp);
        advance();
    }
}

void err(ErrorType errorNum) {
    if (PRINTERR) {
        fprintf(stderr, "error: ");
        switch (errorNum) {
            case DIVZERO_CONST:
                fprintf(stderr, "divide by constant zero\n");
                break;
            case DIVZERO:
                fprintf(stderr, "divide by zero at runtime\n");
                break;
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
                fprintf(stderr, "lvalue required\n");
                break;
            case SYNTAXERR:
                fprintf(stderr, "syntax error\n");
                break;
            default:
                fprintf(stderr, "undefined error\n");
                break;
        }
    }
    printf("EXIT 1\n");
    exit(0);
}
