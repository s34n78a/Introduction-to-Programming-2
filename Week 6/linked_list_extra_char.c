#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _node {
    char s[10005];
    struct _node *next;
} Node;

int main(void) {
    Node *head = NULL;
    Node *tail = NULL;

    Node *P, *temp = head;
    while (temp!=NULL){
        P = temp->next;
        free(temp);
        temp = P;
    }
    return 0;
}