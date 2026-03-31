#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _node {

    int x; // x is the value stored in the node
    struct _node *next; // next is a pointer to the next node in the linked list

} Node;

int main(void) {
    Node *head = NULL; // head is a pointer to the first node in the linked list
    Node *tail = NULL; // tail is a pointer to the last node in the linked list

    // head = (Node *)malloc(sizeof(Node)); // allocate memory for the first node
    // head->x = 1; // set the value of the first node
    // head->next = NULL; // set the next pointer of the first node to NULL
    // tail = head; // set tail to point to the first node
    
    Node *temp = head; // temp is a pointer to the current node in the linked list

    //moving:
    temp = temp->next; // move temp to the next node in the linked list

    //stop
    if (temp->next == NULL)
    {
        printf("stop\n");
    }

    //allocate sapce for the next node
    temp->next = (Node *)malloc(sizeof(Node)); // allocate memory for the next node

    //free next node
    free(temp->next); // free the memory allocated for the next node
    temp->next = NULL; // set the next pointer of the current node to NULL
    
    // insert pointed by head
    Node *new_node = (Node *)malloc(sizeof(Node)); // allocate memory for the new node
    new_node->x = 2; // set the value of the new node
    new_node->next = head; // set the next pointer of the new node to point to the current head
    head = new_node; // update head to point to the new node

    // insert node in the middle
    Node *new_node2 = (Node *)malloc(sizeof(Node)); // allocate memory for the new node
    new_node2->x = 3; // set the value of the new node
    new_node2->next = temp->next; // set the next pointer of the new node to point to the second node in the linked list
    temp->next = new_node2; // update the next pointer of the first node to point to the new node

    //insert node at the end
    Node *new_node3 = (Node *)malloc(sizeof(Node)); // allocate memory for the new node
    new_node3->x = 4; // set the value of the new node
    new_node3->next = NULL; // set the next pointer of the new node to NULL
    tail->next = new_node3; // update the next pointer of the last node to point to the new node
    tail = new_node3; // update tail to point to the new node
    
    //delete node pointed by head
    Node *temp2 = head; // temp2 is a pointer to the node to be deleted
    head = head->next; // update head to point to the next node in the linked list
    free(temp2); // free the memory allocated for the node to be deleted

    //delete node in the middle
    Node *temp3 = temp->next; // temp3 is a pointer to the node to be deleted
    temp->next = temp3->next; // update the next pointer of the first node to point to the node after the node to be deleted
    free(temp3); // free the memory allocated for the node to be deleted
    
    //delete node at the end
    Node *temp4 = tail; // temp4 is a pointer to the node to be deleted
    tail = temp4->next; // update tail to point to the previous node in the linked list
    free(temp4); // free the memory allocated for the node to be deleted
    return 0;

    //move node
    Node *temp5 = head; // temp5 is a pointer to the node to be moved
    head = head->next; // update head to point to the next node in the linked list
    temp5->next = NULL; // set the next pointer of the node to be moved to NULL
    tail->next = temp5; // update the next pointer of the last node to point to the node to be moved
    tail = temp5; // update tail to point to the node to be moved
    return 0;
}