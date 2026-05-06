#ifndef FUNCTION_H
#define FUNCTION_H

struct Node {
    Node *prev;
    Node *next;
    int val;
    int tag;
};

class KuoYangPresent {
public:
    KuoYangPresent(int k);
    void Push(int x);
    void Pop();
    void Reverse();
    void ProgrammingTanoshi();
    void KuoYangTeTe();
    void PrintList();

private:
    Node *head, *mid, *tail;
    int sz, k;
    int now;
    bool reverse;
};

#endif

// Implement your code in a .cpp file
// You can start with the following template:

/*
#include "function.h"

KuoYangPresent::KuoYangPresent(int k) {
	// ...
}
void KuoYangPresent::Push(int x) {
	// ...
}
void KuoYangPresent::Pop() {
	// ...
}
void KuoYangPresent::Reverse() {
    // ...
}
void KuoYangPresent::ProgrammingTanoshi() {
	// ...
}
void KuoYangPresent::KuoYangTeTe() {
	// ...
}
void KuoYangPresent::PrintList() {
    // ...
}
*/