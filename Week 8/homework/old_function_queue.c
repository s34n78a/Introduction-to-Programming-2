#include<stdio.h>
#include<stdlib.h>
#include "function_queue.h"

void solve(Node* head, int l, int r) {
    Node* cur = head->next;
    Node* prev = head;
    Node* before_left = NULL;
    Node* before_right = NULL;
    Node* left = NULL;
    Node* right = NULL;
    Node* next = NULL;
    
    for (int i = 0; i < r; i++)
    {
        //printf("%d\n", cur->v);
        if (i == l-1)
        {
            //printf("found left\n");
            left = cur;
            before_left = prev;
        }
        else if (i >= l)
        {
            next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        else
        {
            cur = cur->next;
            prev = prev->next;
        }
        
        if (i == r-2)
        {
            before_right = cur;
        }
    }
    right = cur;
    //printf("before left:%d->%d, left: %d->%d, before right:%d->%d, right: %d->%d\n", before_left->v, before_left->next->v, left->v, left->next->v, before_right->v, before_right->next->v, right->v, right->next->v);
    
    Node* next_right = right->next;
    left->next = right->next;
    before_left->next = right;
    right->next = before_right;
    //printf("before left:%d->%d, left: %d->%d, before right:%d->%d, right: %d->%d\n", before_left->v, before_left->next->v, left->v, left->next->v, before_right->v, before_right->next->v, right->v, right->next->v);

//     cur = head->next;
//     for (int i = 1; i < r+2; i++)
//     {
//         printf("position %d: %d\n", i, cur->v);
//         cur = cur->next;
//     }
}
