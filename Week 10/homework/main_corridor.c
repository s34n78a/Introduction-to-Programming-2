#include <stdio.h>
#include <stdlib.h>
#include "function_corridor.h"

/**
 * This file is handled by the server and cannot be modified by students.
 * It reads the room layout, builds the linked list, and calls detectCycle().
 */
int main() {
    int n;
    scanf("%d", &n);

    int *nxt = (int *)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) scanf("%d", &nxt[i]);

    /* Build linked list: nodes[i] represents room i */
    Node *nodes = (Node *)malloc((n + 1) * sizeof(Node));
    for (int i = 1; i <= n; i++)
        nodes[i].next = (nxt[i] == 0) ? NULL : &nodes[nxt[i]];

    printf("%d\n", (int)(detectCycle(&nodes[1]) - nodes));

    free(nxt);
    free(nodes);
    return 0;
}
