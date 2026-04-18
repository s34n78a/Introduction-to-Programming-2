#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {ID_A, ID_B, ID_C, ID_D,
OP_AND, OP_OR} TokenSet;

char sym[6]="ABCD&|";
typedef struct _Node {
    TokenSet data;
    struct _Node *left, *right;
} BTNode;

BTNode* pre_parser()
{
    int i;
    BTNode *node;
    static pos=0; // a static variable to remember the current position in the expr[]
    if (expr[pos]>='A' && expr[pos]<='D') // prefix = ID
    return makeNode(expr[pos++]);
    else{ // prefix = OP <prefix> <prefix>
        node = makeNode(expr[pos++]);
        node->left = pre_parser();
        node->right = pre_parser();

        return node;
    }
}