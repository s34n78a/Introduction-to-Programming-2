#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_station.h"

Node* heads[100005] = {NULL};
Node* tails[100005] = {NULL};

int main() {
    // Input the total number of platforms and the number of operations.
    int M, Q;
    if (scanf("%d %d", &M, &Q) != 2) {
        return 0;
    }

    char cmd[15];
    while (Q--) {
        scanf("%s", cmd);

        if (cmd[0] == 'E') {
            // ENTER p c
            int p, c;
            scanf("%d %d", &p, &c);
            enter(p, c);
        } 
        else if (cmd[0] == 'M') {
            // MERGE p_src p_dest
            int p_src, p_dest;
            scanf("%d %d", &p_src, &p_dest);
            merge(p_src, p_dest);
        } 
        else if (cmd[0] == 'S') {
            // SPLIT p_src c p_dest
            int p_src, c, p_dest;
            scanf("%d %d %d", &p_src, &c, &p_dest);
            split(p_src, c, p_dest);
        } 
        else if (cmd[0] == 'R') {
            // REVERSE p
            int p;
            scanf("%d", &p);
            reverse(p);
        } 
        else if (cmd[0] == 'C') {
            // CHECK p c k
            int p, c, k;
            scanf("%d %d %d", &p, &c, &k);
            printf("%d\n", check(p, c, k));
        }
    }

    // Output the final sequence of train cars on each non-empty platform
    for (int i = 1; i <= M; i++) {
        if (heads[i] != NULL) {
            printf("%d:", i);
            Node* curr = heads[i];
            while (curr != NULL) {
                printf(" %d", curr->id);
                curr = curr->next;
            }
            printf("\n");
        }
    }

    return 0;
}