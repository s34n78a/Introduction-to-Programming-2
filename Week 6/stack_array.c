#include <stdio.h>
#include "stack_array.h"

int stack[MAX];
int top = -1;

void push(int x) {
    if (top == MAX - 1) {
        printf("Stack Overflow\n");
        return;
    }
    
    stack[++top] = x;
}

int pop() {
    if (top == -1) {
        printf("Stack Underflow\n");
        return -1;
    }
    
    return stack[top--];
}

void display() {
    if (top == -1) {
        printf("Stack is empty\n");
        return;
    }
    
    for (int i = top; i >= 0; i--) {
        printf("%d ", stack[i]);
    }
    
    printf("\n");
}