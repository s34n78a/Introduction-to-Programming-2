#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int id;
    struct node *next;
} Node;

void insert(Node *head, int id) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    new_node->id = id;
    new_node->next = head->next;
    head->next = new_node;
}

int main(void) {
    Node *head;
    head = (Node*) malloc(sizeof(Node));
    if (head == NULL) {
        return 1;
    }
    head->id = 1;
    head->next = head;

    free(head);
    return 0;
}