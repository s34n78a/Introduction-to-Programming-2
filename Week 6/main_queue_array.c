#include <stdio.h>
#include "queue_array.h"
int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    
    display();
    
    printf("Dequeued: %d\n", dequeue());
    display();
    
    return 0;
}