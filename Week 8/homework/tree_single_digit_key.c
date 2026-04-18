#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char value;
    struct Node *left;
    struct Node *right;
} Node;

static Node *new_node(char value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static Node *parse_tree_a(const char *s, int *idx) {
    if (s[*idx] != '(') {
        return NULL;
    }

    (*idx)++;  // '('
    Node *root = new_node(s[*idx]);
    (*idx)++;  // single-digit value

    if (s[*idx] == '/') {
        (*idx)++;  // '/'
    }

    if (s[*idx] == '(') {
        root->left = parse_tree_a(s, idx);
    }

    if (s[*idx] == '/') {
        (*idx)++;  // '/'
    }

    if (s[*idx] == '(') {
        root->right = parse_tree_a(s, idx);
    }

    if (s[*idx] == ')') {
        (*idx)++;  // ')'
    }

    return root;
}

static Node *parse_tree_b(const char *s, int *idx);

static Node *parse_subtree_b(const char *s, int *idx) {
    if (s[*idx] != '(') {
        return NULL;
    }
    (*idx)++;  // '('

    if (s[*idx] == ')') {
        (*idx)++;  // ')'
        return NULL;
    }

    Node *subtree = parse_tree_b(s, idx);

    if (s[*idx] == ')') {
        (*idx)++;  // ')'
    }

    return subtree;
}

static Node *parse_tree_b(const char *s, int *idx) {
    Node *root = new_node(s[*idx]);
    (*idx)++;  // single-digit value

    root->left = parse_subtree_b(s, idx);
    root->right = parse_subtree_b(s, idx);

    return root;
}

static int is_same_tree(const Node *a, const Node *b) {
    if (a == NULL && b == NULL) {
        return 1;
    }
    if (a == NULL || b == NULL) {
        return 0;
    }
    if (a->value != b->value) {
        return 0;
    }
    return is_same_tree(a->left, b->left) && is_same_tree(a->right, b->right);
}

static void free_tree(Node *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        return 0;
    }

    char *a = (char *)malloc(400005 * sizeof(char));
    char *b = (char *)malloc(400005 * sizeof(char));

    if (scanf("%400004s", a) != 1) {
        free(a);
        free(b);
        return 0;
    }
    if (scanf("%400004s", b) != 1) {
        free(a);
        free(b);
        return 0;
    }

    int idx_a = 0;
    int idx_b = 0;

    Node *tree_a = parse_tree_a(a, &idx_a);
    Node *tree_b = parse_tree_b(b, &idx_b);

    puts(is_same_tree(tree_a, tree_b) ? "YES" : "NO");

    free_tree(tree_a);
    free_tree(tree_b);
    free(a);
    free(b);

    return 0;
}
