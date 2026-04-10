#include <stdio.h>

typedef struct _Node {
    int id, tolerance;
    struct _Node* next, *prev; // next pointer points to the next node, prev pointer points to the previous node in the doubly linked list
} Node;

void insert_before_temp(Node* temp, int id, int tolerance) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    new_node->id = id;
    new_node->tolerance = tolerance;
    
    new_node->next = temp;
    new_node->prev = temp->prev;

    if (temp->prev != NULL) {
        temp->prev->next = new_node;
    }
    temp->prev = new_node;
}

void insert_after_temp(Node* temp, int id, int tolerance) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return;
    }
    new_node->id = id;
    new_node->tolerance = tolerance;

    new_node->prev = temp;
    new_node->next = temp->next;

    if (temp->next != NULL) {
        temp->next->prev = new_node;
    }
    temp->next = new_node;
}

void delete_before_temp(Node* temp) {
    if (temp->prev == NULL) {
        return;
    }
    Node* to_delete = temp->prev;
    temp->prev = to_delete->prev;

    if (to_delete->prev != NULL) {
        to_delete->prev->next = temp;
    }
    free(to_delete);
}

void delete_after_temp(Node* temp) {
    if (temp->next == NULL) {
        return;
    }
    Node* to_delete = temp->next;
    temp->next = to_delete->next;

    if (to_delete->next != NULL) {
        to_delete->next->prev = temp;
    }
    free(to_delete);
}

Node* createList(int n);
void solve(Node* head, int N, int K);

int main(void) {
    int N, K;
    if (scanf("%d %d", &N, &K) != 2 || N <= 0 || K <= 0) {
        return 1;
    }

    Node* head = createList(N);
    if (head == NULL) {
        return 1;
    }

    solve(head, N, K);

    // Free the remaining node
    free(head);
    return 0;
}