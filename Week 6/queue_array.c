#include <stdio.h>
#include "queue_array.h"

int queue[MAX];
int front = 0;
int rear = -1;

void enqueue(int x) {
    if (rear == MAX - 1) {
        printf("Queue Overflow\n");
        return;
    }
    
    queue[++rear] = x;
}

int dequeue() {
    if (front > rear) {
        printf("Queue Underflow\n");
        return -1;
    }
    
    return queue[front++];
}
void display() {
    if (front > rear) {
        printf("Queue is empty\n");
        return;
    }
    
    for (int i = front; i <= rear; i++) {
        printf("%d ", queue[i]);
    }
    
    printf("\n");
}