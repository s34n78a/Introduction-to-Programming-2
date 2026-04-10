#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int id;
    int next;
} Node;

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    Node *people = (Node*)malloc(n * sizeof(Node));
    if (people == NULL) {
        return 1;
    }

    for (int i = 0; i < n; i++) {
        people[i].id = i + 1;
        people[i].next = (i + 1) % n;
    }

    int step = 3;
    int alive = n;
    int current = 0;
    int prev = n - 1;

    while (alive > 1) {
        for (int i = 1; i < step; i++) {
            prev = current;
            current = people[current].next;
        }

        printf("People %d is killed\n", people[current].id);
        people[prev].next = people[current].next;
        current = people[current].next;
        alive--;
    }
    printf("People %d survived\n", people[current].id);

    free(people);
    return 0;
}