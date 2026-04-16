#include<stdio.h>
#include<stdlib.h>
#include "function_queue.h"

void solve(Node* head, int l, int r) {
    if (head == NULL || head->next == NULL || l >= r) {
        return;
    }

    Node* prev = head;
    for (int pos = 1; pos < l; pos++) {
        prev = prev->next;
    }

    Node* sub_head = prev->next;
    Node* cur = sub_head->next;

    for (int i = 0; i < r - l; i++) {
        sub_head->next = cur->next;
        cur->next = prev->next;
        prev->next = cur;
        cur = sub_head->next;
    }
}
