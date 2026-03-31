#include <stdio.h>
#include "stack_array.h"

int main() {
    push(10);
    push(20);
    push(30);

    display();
    
    printf("Popped: %d\n", pop());
    display();
    
    return 0;
}