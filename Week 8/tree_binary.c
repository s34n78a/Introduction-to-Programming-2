#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
    int data;
    struct _node *left, *right;
} BTNode;

void preorder(BTNode *root){
    if(root!=NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(BTNode *root){
    if(root!=NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void postorder(BTNode *root){
    if(root!=NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int maxValue(BTNode *root) {
    if(root==NULL) return -1;
    int max = root->data;
    int leftMax = maxValue(root->left);
    int rightMax = maxValue(root->right);
    if(leftMax > max) max = leftMax;
    if(rightMax > max) max = rightMax;
    return max;
}

void freeTree(BTNode *root) {
    if(root!=NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void printTree(BTNode *root, int level) {
    if(root!=NULL) {
        printTree(root->right, level + 1);
        for(int i=0; i<level; i++) printf("    ");
        printf("%d\n", root->data);
        printTree(root->left, level + 1);
    }
}

void sortTree(BTNode *root) {
    if(root!=NULL) {
        sortTree(root->left);
        sortTree(root->right);
        if(root->left!=NULL && root->left->data > root->data) {
            int temp = root->data;
            root->data = root->left->data;
            root->left->data = temp;
        }
        if(root->right!=NULL && root->right->data < root->data) {
            int temp = root->data;
            root->data = root->right->data;
            root->right->data = temp;
        }
    }
}

void heapSort(BTNode *root) {
    // This function is a placeholder for heap sort implementation on the binary tree.
    // Implementing heap sort on a binary tree is non-trivial and typically requires converting the tree to an array first, then performing heap sort on the array.
}

int main(void) {
    BTNode *root = (BTNode*)malloc(sizeof(BTNode));
    char data[] = {1, 2, 3, 4, 5, 6, 7};
    root->data = data[0];
    root->left = (BTNode*)malloc(sizeof(BTNode));
    root->right = (BTNode*)malloc(sizeof(BTNode));
    root->left->data = data[1];
    root->left->left = (BTNode*)malloc(sizeof(BTNode));
    root->left->right = (BTNode*)malloc(sizeof(BTNode));
    root->left->left->data = data[3];
    root->left->left->left = NULL;
    root->left->left->right = NULL;
    root->left->right->data = data[4];
    root->left->right->left = NULL;
    root->left->right->right = NULL;
    root->right->data = data[2];
    root->right->left = (BTNode*)malloc(sizeof(BTNode));
    root->right->right = (BTNode*)malloc(sizeof(BTNode));
    root->right->left->data = data[5];
    root->right->left->left = NULL;
    root->right->left->right = NULL;
    root->right->right->data = data[6];
    root->right->right->left = NULL;
    root->right->right->right = NULL;

    printf("Preorder: ");
    preorder(root);
    printf("\n");

    printf("Inorder: ");
    inorder(root);
    printf("\n");

    printf("Postorder: ");
    postorder(root);
    printf("\n");

    printf("Max value: %d\n", maxValue(root));

    printTree(root, 0);

    sortTree(root);
    printf("Sorted Tree:\n");
    printTree(root, 0);

    freeTree(root);
    return 0;
}