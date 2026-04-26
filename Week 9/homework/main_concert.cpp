#include <iostream>
#include "function.h"

// helper function for creating new node
static ListNode* newNode(int val) {
    ListNode* ret = new ListNode();
    // const removed to simplify data type explanation
    ret->value = val;
    ret->next = nullptr;
    return ret;
}

int main(void) 
{
    // a common practice for better I/O performance
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // input line 1
    int N;
    std::cin >> N;

    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    
    int value, chk = 0;
    for (int i = 0; i < N; ++i) {
        std::cin >> value;

        ListNode* node = newNode(value);

        chk ^= (reinterpret_cast<intptr_t>(node) + node->value); // if have interest in this check out: 

        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    // implementation tested
    ListNode* result = solve(head);

    int chk2 = 0;
    // print out the output
    ListNode* curr = result;
    while (curr != nullptr) {
        chk2 ^= (reinterpret_cast<intptr_t>(curr) + curr->value);

        std::cout << curr->value;
        if (curr->next != nullptr) {
            std::cout << " ";
        }
        curr = curr->next;
    }
    std::cout << "\n";

    if (chk != chk2) std::cout << "Wrong Answer\n";

    // cleanup
    curr = result;
    while (curr != nullptr) {
        ListNode* tmp = curr;
        curr = curr->next;
        delete tmp;
    }

    return 0;
}
