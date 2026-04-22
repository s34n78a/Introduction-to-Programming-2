#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "function_expr.h"

#define MAXEXPR 256
#define NUMSYM 6

char expr[MAXEXPR];  // string to store the input expression.
int pos;             // current position of parsing, from end to start

char sym[NUMSYM];

BTNode* EXPR() {
    BTNode *right = FACTOR();

    if (pos >= 0 && (expr[pos] == '&' || expr[pos] == '|')) {
        char op = expr[pos--];
        BTNode *node = makeNode(op);
        node->right = right;
        node->left = EXPR();
        return node;
    }

    return right;
}

BTNode* FACTOR() {
    if (pos < 0) {
        return NULL;
    }

    if (expr[pos] >= 'A' && expr[pos] <= 'D') {
        return makeNode(expr[pos--]);
    }

    if (expr[pos] == ')') {
        pos--;
        BTNode *node = EXPR();
        if (pos >= 0 && expr[pos] == '(') {
            pos--;
        }
        return node;
    }

    return NULL;
}

BTNode* makeNode(char c) {
    BTNode* new_node = (BTNode*)malloc(sizeof(BTNode));
    if (new_node == NULL) {
        return NULL;
    }

    for (int i = 0; i < 6; i++)
    {
        if (sym[i] == c)
        {
            new_node->data = i;
            break;
        }   
    }

    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}
