#include "function_stack.h"
#include <iostream>
#include <string.h>

using namespace std;

List_stack::List_stack() {
    head = NULL;
    tail = NULL;
}

void List_stack::push(const int &value) {
    if (head == NULL)
    {
        head = new ListNode(value);
        tail = head;
    }
    else
    {
        ListNode *temp = new ListNode(value);
        tail->nextPtr = temp;
        temp->prevPtr = tail;
        tail = temp;
    }
}

void List_stack::pop() {
    if (tail == NULL)
    {
        return;
    }
    else if (tail == head)
    {
        delete tail;
        head = NULL;
        tail = NULL;
    }
    else
    {
        ListNode *temp = tail;
        tail = temp->prevPtr;
        delete temp;
        tail->nextPtr = NULL;
    }
    
}

void List_stack::print() {
    if (tail == NULL)
    {
        return;
    }

    ListNode *temp = tail;
    while (temp->prevPtr != NULL)
    {
        std::cout << temp->data << " ";
        temp = temp->prevPtr;
    }
    std::cout << temp->data;
}

List_stack::~List_stack() {
    ListNode *temp = tail;
    while (temp != head)
    {
        temp = temp->prevPtr;
        delete temp->nextPtr;
    }
    delete temp;
}