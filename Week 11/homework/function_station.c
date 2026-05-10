#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_station.h"

Node *nodes[1000005];

// 1. ENTER p c
void enter(int p, int c) {
    if (heads[p] == NULL) {
        heads[p] = (Node*)malloc(sizeof(Node));
        heads[p]->id = c;
        heads[p]->prev = NULL;
        heads[p]->next = NULL;
        tails[p] = heads[p];
    }
    else
    {
        tails[p]->next = (Node*)malloc(sizeof(Node));
        tails[p]->next->id = c;
        tails[p]->next->prev = tails[p];
        tails[p]->next->next = NULL;
        tails[p] = tails[p]->next;
    }

    nodes[c] = tails[p];
}

// 2. MERGE p_src p_dest
void merge(int p_src, int p_dest) {
    if (heads[p_src] == NULL) {
        return;
    }

    if (heads[p_dest] == NULL)
    {
        heads[p_dest] = heads[p_src];
        tails[p_dest] = tails[p_src];
    }
    else
    {
        tails[p_dest]->next = heads[p_src];
        heads[p_src]->prev = tails[p_dest];
        tails[p_dest] = tails[p_src];
    }
    heads[p_src] = NULL;
    tails[p_src] = NULL;
}

// 3. SPLIT p_src c p_dest
void split(int p_src, int c, int p_dest) {
    Node* target = nodes[c];

    if (target == heads[p_src])
    {
        merge(p_src, p_dest);
        return;
    }
    else
    {
        heads[p_dest] = target;
        tails[p_dest] = tails[p_src];
        tails[p_src] = target->prev;
        tails[p_src]->next = NULL;
        target->prev = NULL;
    }
}

// 4. REVERSE p
void reverse(int p) {
    Node* curr = heads[p];
    Node* temp = NULL;
    while (curr != NULL) {
        temp = curr->next;
        curr->next = curr->prev;
        curr->prev = temp;
        curr = temp;
    }
    temp = heads[p];
    heads[p] = tails[p];
    tails[p] = temp;
}

// 5. CHECK p c k
int check(int p, int c, int k) {
    Node* cur = nodes[c];

    for (int i = k; i > 0; i--)
    {
        if (cur->prev == NULL)
        {
            return cur->id;
        }
        cur = cur->prev;
    }
    
    return cur->id;
}
