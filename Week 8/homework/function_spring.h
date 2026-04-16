#include <stdio.h>

typedef struct _Node {
    int id, tolerance;
    struct _Node* next, *prev;
} Node;

Node* createList(int n);
void solve(Node* head, int N, int K);
