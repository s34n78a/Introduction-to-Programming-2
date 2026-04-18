#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node ;

Node* create_new_node(int value) {
    Node *new_node = (Node*)calloc(sizeof(Node), 1);
    
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

int num_reader(const char* string, int* idx) {
    int num = 0;

    int i = *idx;
    while ('0' <= string[*idx] && '9' >= string[*idx] && *idx < strlen(string))
    {
        num = num*10 + (string[i] - '0');
        (*idx)++;
    }
    return num;   
}

Node* parse_tree_x(const char* string_x, int* id_x) {
    // printf("%c\n", string_x[*id_x]);
    (*id_x)++;

    // printf("%c\n", string_x[*id_x]);
    // printf("Found first num\n");
    int num = num_reader(string_x, id_x);
    
    Node* tree_x = create_new_node(num);
    // printf("Tree x created with value %d\n", tree_x->value);

    // printf("%c\n", string_x[*id_x]);
    if (string_x[*id_x] == '/')
    {
        (*id_x)++;
    }
    
    // printf("%c\n", string_x[*id_x]);
    if (string_x[*id_x] == '(')
    {
        // printf("Parsing left subtree\n");
        tree_x->left = parse_tree_x(string_x, id_x);
    }
    
    // printf("%c\n", string_x[*id_x]);
    if (string_x[*id_x] == '/')
    {
        (*id_x)++;
    }
    
    // printf("%c\n", string_x[*id_x]);
    if (string_x[*id_x] == '(')
    {
        // printf("Parsing right subtree\n");
        tree_x->right = parse_tree_x(string_x, id_x);
    }
    
    // printf("%c\n", string_x[*id_x]);
    if (string_x[*id_x] == ')')
    {
        // printf("Finished Parsing subtree\n");
        (*id_x)++;
    }
    
    return tree_x;
}

Node* parse_tree_y(const char* string_y, int* id_y) {
    // printf("%c\n", string_y[*id_y]);
    int num = num_reader(string_y, id_y);

    Node* tree_y = create_new_node(num);
    // printf("Tree y created with value %d\n", tree_y->value);

    (*id_y)++;

    // printf("%c\n", string_y[*id_y]);
    if ('0' <= string_y[*id_y] && '9' >= string_y[*id_y])
    {
        // printf("Parsing left subtree\n");
        tree_y->left = parse_tree_y(string_y, id_y);
    }
    
    // printf("%c\n", string_y[*id_y]);
    if (string_y[*id_y] == ')')
    {
        // printf("Finished Parsing left subtree\n");
        (*id_y)++;
    }
    
    // printf("%c\n", string_y[*id_y]);
    if (string_y[*id_y] == '(')
    {
        (*id_y)++;
    }

    // printf("%c\n", string_y[*id_y]);
    if ('0' <= string_y[*id_y] && '9' >= string_y[*id_y])
    {
        // printf("Parsing right subtree\n");
        tree_y->right = parse_tree_y(string_y, id_y);
    }
    
    // printf("%c\n", string_y[*id_y]);
    if (string_y[*id_y] == ')')
    {
        // printf("Finished Parsing right subtree\n");
        (*id_y)++;
    }
    
    // printf("%c\n", string_y[*id_y]);
    if (string_y[*id_y] == ')')
    {
        // printf("Finished Parsing tree\n");
        (*id_y)++;
    }

    return tree_y;
}

int is_same(const Node* tree_x, const Node* tree_y) {
    if (tree_x == NULL && tree_y == NULL) {
        // printf("Both null\n");
        return 1;
    }
    else if (tree_x == NULL || tree_y == NULL) {
        // printf("One null\n");
        return 0;
    }
    else if (tree_x->value != tree_y->value) {
        // printf("Different Value\n");
        return 0;
    }
    // printf("Recursive check\n");
    return is_same(tree_x->left, tree_y->left) && is_same(tree_x->right, tree_y->right);
}

void free_tree(Node* root) {
    if (root == NULL) {
        // printf("Node is null...\n");
        return;
    }

    // printf("Recursive left node\n");
    free_tree(root->left);

    // printf("Recursive right node\n");
    free_tree(root->right);

    // printf("Final free node with value %d\n", root->value);
    free(root);
}

int main(void) {
    int n = 0;
    char x[1000000];
    char y[1000000];

    scanf("%d", &n);
    scanf("%s", x);
    scanf("%s", y);
    
    int id_x = 0;
    int id_y = 0;

    // printf("Parsing x...\n");
    Node* tree_x = parse_tree_x(x, &id_x);

    // printf("Parsing y...\n");
    Node* tree_y = parse_tree_y(y, &id_y);

    // printf("Checking...\n");
    int same_check = is_same(tree_x, tree_y);
    //printf("%d\n", same_check);
    if (same_check == 1) {
        printf("YES");
    }
    else {
        printf("NO");
    }
    
    // printf("\nFreeing Tree x\n");
    free_tree(tree_x);
    // printf("\nFreeing Tree y\n");
    free_tree(tree_y);
    
    return 0;
}