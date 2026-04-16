#include <stdio.h>
#include <stdlib.h>
#include "function_spring.h"

Node* createList(int n) {
    Node* head = (Node*)malloc(sizeof(Node));
    Node* current = head;
    Node* previous = NULL;

    for (int i = 0; i < n; i++)
    {
        current->id = i+1;
        scanf("%d", &current->tolerance);

        current->next = (Node*)malloc(sizeof(Node));

        previous = current;
        current = current->next;
        current->prev = previous;

        //check input
        // printf("Node %d: tolerance %d\n", previous->id, previous->tolerance);
    }
    current = previous;
    current->id = n;
    current->next = head;
    head->prev = current;
    //printf("Node %d: tolerance %d\n", current->id, current->tolerance);

    return head;
}

void solve(Node* head, int N, int K) {
    Node* current = head;
    Node* killed = NULL;
    Node* previous = NULL;
    int go_cw = 0;

    int total_alive = head->prev->id;
    //printf("total node: %d\n", total_alive);

    // check linked list
    // for (int i = 0; i < N; i++)
    // {
    //     printf("Node %d: tolerance %d, next id %d, prev id %d\n", current->id, current->tolerance, current->next->id, current->prev->id);
    //     current = current->next;
    // }
    // printf("\n");

    current = head;
    int step = K;
    while (total_alive > 1)
    {
        // printf("step: %d\n", step);
        if (step > 0)
        {
            step--;
            go_cw = 1;
        }
        else if (step < 0)
        {
            step++;
            go_cw = 0;
        }
        step = step % total_alive;
        // printf("change step: %d\n", step);
        
        while (step != 0)
        {
            // printf("Node %d: tolerance %d, next id %d, prev id %d\n", current->id, current->tolerance, current->next->id, current->prev->id);
            
            // printf("step: %d\n", step);
            if (step > 0)
            {
                current = current->next;
                step--;
            }
            else if (step < 0)
            {
                current = current->prev;
                step++;
            }
            
        }
        printf("%d ", current->id);

        // printf("Kill node %d: tolerance %d, next id %d, prev id %d\n", current->id, current->tolerance, current->next->id, current->prev->id);

        previous = current->prev;
        current->prev->next = current->next;
        current->next->prev = previous;

        killed = current;
        step = current->tolerance;
        
        if (go_cw)
        {
            current = current->next;
        }
        else
        {
            current = current->prev;
        }
        // printf("Start node %d: tolerance %d, next id %d, prev id %d\n", current->id, current->tolerance, current->next->id, current->prev->id);
        
        free(killed);

        total_alive--;
    }
    printf("\n%d\n", current->id);
    
    
    return;
}