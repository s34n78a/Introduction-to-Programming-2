#ifndef _FUNCTION_H_
#define _FUNCTION_H_

typedef struct Node{
    const int v;
    struct Node *next;
}Node;
void solve(Node* head, int l, int r);

#endif