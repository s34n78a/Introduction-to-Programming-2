#include <stdio.h>
#include "stack_linked.h"

int main() {
    push(5);
    push(15);
    push(25);

    display();

    printf("Popped: %d\n", pop());
    display();

    return 0;
}