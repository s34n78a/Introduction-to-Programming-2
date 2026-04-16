#include <stdio.h>
#include <stdlib.h>
#include "function_spring.h"

int main(void) {
    int N, K;
    scanf("%d %d", &N, &K);

    // build the doubly ll
    Node* head = createList(N);

    solve(head, N, K);
    
    return 0;
}
