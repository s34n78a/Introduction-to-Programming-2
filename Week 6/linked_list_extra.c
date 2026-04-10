#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _node {
    int data;
    struct _node *next;
} Node;

int read_in_data() {
    // This function should read data from input and return it as an integer.
    // For demonstration purposes, let's just return a dummy value.
    return 0; // Replace with actual data reading logic.
}

int main(void) {
    Node *head, *temp;
    head = (Node*) malloc(sizeof(Node));
    head->data = read_in_data();
    head->next = NULL;
    temp = head;

    while (temp->data != -1) { // Assuming -1 is the sentinel value to stop reading
        temp->next = (Node*) malloc(sizeof(Node));
        temp = temp->next;
        temp->data = read_in_data();
        temp->next = NULL;
    }


    return 0;
}