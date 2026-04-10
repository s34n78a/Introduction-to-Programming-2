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

    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        free(head);
        return 1;
    }

    // Insert nodes with ids from 2 to n
    for (int i = 2; i <= n; i++) {
        insert(head, i);
    }

    // connect the last node to head to form a circular linked list
    Node *current = head;
    while (current->next != head) {
        current = current->next;
    }
    current->next = head;

    int step = 3;

    Node *prev = head;
    current = head;
    while (prev->next != head) {
        prev = prev->next;
    }

    while (current->next != current) {
        for (int i = 1; i < step; i++) {
            prev = current;
            current = current->next;
        }

        printf("People %d is killed\n", current->id);
        Node *to_delete = current;
        prev->next = current->next;
        current = current->next;

        if (to_delete == head) {
            head = current;
        }
        free(to_delete);
    }
    printf("People %d survived\n", current->id);

    free(current);
    return 0;
}