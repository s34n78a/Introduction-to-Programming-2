#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int alive;
} People;

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    People *people = malloc(n * sizeof(People));
    if (people == NULL) {
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        people[i].id = i + 1;
        people[i].alive = 1;
    }
    
    int step = 3;
    int current = 0;
    int count = 0;
    int num_of_alive = n;

    while (num_of_alive > 1)
    {
        if (people[current].alive == 1) {
            count++;
            if (count == step) {
                people[current].alive = 0;
                printf("People %d is killed\n", people[current].id);
                num_of_alive--;
                count = 0;
            }
        }

        current = (current + 1) % n;
    }
    
    for (int i = 0; i < n; i++)
    {
        if (people[i].alive == 1)
        {
            printf("People %d survived\n", people[i].id);
        }
    }

    free(people);
    return 0;
}