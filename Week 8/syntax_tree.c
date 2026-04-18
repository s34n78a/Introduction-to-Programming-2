#include <stdio.h>
#include <stdlib.h>

typedef enum {ID_A, ID_B, ID_C, ID_D,
OP_AND, OP_OR} TokenSet;

char sym[6]="ABCD&|";
typedef struct _Node {
    TokenSet data;
    struct _Node *left, *right;
} BTNode;

/*Auxiliary function to make a node*/
BTNode* makeNode(char value){
    BTNode *node = (BTNode*) malloc(sizeof(BTNode));
    int i;
    for(i=0;i<6;i++) {
        if (value==sym[i]) {
            node->data = i;
        }
    }
    node->left = NULL; 
    node->right = NULL;
    return node;
}

BTNode* EXPR(){
    char c;
    BTNode *node = NULL, *right=NULL;
    if (pos>=0) { // if the expression has length >= 1.
        c = expr[pos--]; // parse from the end of expression.
        if (c>= 'A' && c<='D') {// Get an ID
            right = makeNode(c);
        }

        if (pos>0) { // 1. the EXPR OP ID case
            c = expr[pos];
            if (c=='&' || c=='|'){
                node = makeNode(c);
                node->right = right;
                pos--;
                node->left = EXPR();
            } else {
                node = right; // 2. EXPR = ID
            }
        } else {
            node = right; // 2. EXPR = ID
        }
    }
    return node;
}

BTNode* FACTOR(){
    char c;
    BTNode *node = NULL;

    if (pos>=0) {
        c = expr[pos--];
        if (c>= 'A' && c<='D'){// factor = ID
            node = makeNode(c);
        } else if (c==')') { // factor = (EXPR)
            node = EXPR();
            if(expr[pos--]!= '(') {
                printf("Error!\n");
                freeTree(node);
            }
        }
    }
    return node;
}

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


int main(void) {
    return 0;
}