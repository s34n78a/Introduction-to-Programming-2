#include <stdio.h>
#include <string.h>

/* maximum no. of characters */
#define MAX 100

/* stack variables */
int top = -1;
int item;

char stack_string[MAX];

void pushChar(char item);
char popChar(void);
int isEmpty(void);
int isFull(void);

int main() {
    char str[MAX];
    int i;

    printf("Input a string: ");
    // Reads string until a newline is encountered
    scanf("%[^\n]s", str);

    // Push each character into the stack
    for(i = 0; i < strlen(str); i++) {
        pushChar(str[i]);
    }

    // Pop characters back into the string (reversing it)
    for(i = 0; i < strlen(str); i++) {
        str[i] = popChar();
    }

    printf("Reversed String is: %s\n", str);

    return 0;
}

void pushChar(char item) {
    //check if stack is full
    if(isFull()) {
        printf("Stack is full. Cannot push '%c'\n", item);
        return;
    } 
    
    //increment top and add item to stack
    top = top + 1;
    stack_string[top] = item;
}

char popChar() {
    //check if stack is empty
    if(isEmpty()) {
        printf("Stack is empty. Cannot pop.\n");
        return '\0'; // Return null character if stack is empty
    }
    
    //return top item and decrement top
    char item = stack_string[top];
    top = top - 1;
    return item;
}

int isEmpty() {
    if (top == -1)
    {
        return 1; // Stack is empty
    } 
    
    else
    {
        return 0; // Stack is not empty
    }
}

int isFull() {
    if (top == MAX - 1)
    {
        return 1; // Stack is full
    } 
    
    else
    {
        return 0; // Stack is not full
    }
}