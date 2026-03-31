#include <stdio.h>
#include "queue_linked.h"
int main() {
    enqueue(5);
    enqueue(15);
    enqueue(25);
    
    display();
    
    printf("Dequeued: %d\n", dequeue());
    display();
    
    return 0;
}