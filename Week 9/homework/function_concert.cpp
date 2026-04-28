#include <iostream>
#include "function_concert.h"

struct Node
{
    // long long id;
    struct Node* next;
    struct Node* prev;
    struct ListNode* list_node;
};

void printListNode(ListNode* head) {
    // std::cout << "printListNode: \n";

    ListNode* cur = head;
    while (cur != NULL)
    {
        if (cur->value)
        {
            std::cout << cur->value << ' ';
        }
        else
        {
            std::cout << "null ";
        }

        if (cur->next != NULL)
        {
            std::cout << "next: " << cur->next->value << '\n';
        }
        else
        {
            std::cout << "next: null\n";
        }
        
        
        cur = cur->next;
    }
}

void printNode(Node* head) {
    Node* cur = head;
    if (cur == NULL) {
        std::cout << "printNode: null\n";
        return;
    }

    std::cout << "printNode: \n";

    while (cur != NULL)
    {
        // if (cur->id >= 0)
        // {
        //     std::cout << cur->id << ' ';
        // }
        // else
        // {
        //     std::cout << "null ";
        // }

        if (cur->list_node != NULL)
        {
            std::cout << "list_node: " << cur->list_node->value << ' ';
        }
        else
        {
            std::cout << "list_node: null ";
        }
        // if (cur->next != NULL)
        // {
        //     std::cout << "next: " << cur->next->id << ' ';
        // }
        // else
        // {
        //     std::cout << "next: null ";
        // }
        // if (cur->prev != NULL)
        // {
        //     std::cout << "prev: " << cur->prev->id << '\n';
        // }
        // else
        // {
        //     std::cout << "prev: null\n";
        // }
        cur = cur->next;
    }
}

Node* read_newNode(long long val, ListNode* new_list_node) {
    Node* ret = new Node();
    // const removed to simplify data type explanation
    // ret->id = val;
    ret->list_node = new_list_node;
    ret->next = NULL;
    ret->prev = NULL;
    return ret;
}

void free_read_newNode(Node* head) {
    Node* cur = head;
    while (cur->next != NULL)
    {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    delete cur;
}

ListNode* solve(ListNode* head) {
    // std::cout << "solve: \n";
    // printListNode(head);

    long long counter = 0;
    ListNode* list_cur = head;
    Node* read_head = NULL;
    Node* cur = read_head;
    Node* prev = NULL;

    while (list_cur != NULL)
    {
        // std::cout << "counter: " << counter << '\n';
        if (counter == 0) {
            read_head = read_newNode(counter+1, list_cur);
            cur = read_head;
        } else {
            cur = read_newNode(counter+1, list_cur);
        }

        // std::cout << "cur: " << cur->id << '\n';
        if (prev != NULL) {
            prev->next = cur;
        }

        list_cur = list_cur->next;
        
        // printNode(read_head);

        cur->prev = prev;
        prev = cur;
        cur = cur->next;

        counter++;
    }
    
    // std::cout << counter << '\n';
    // std::cout << "prev: " << prev->id << '\n';
    // printNode(read_head);

    Node* tail = prev;
    cur = read_head;

    while (cur->list_node != tail->list_node)
    {
        cur->list_node->next = tail->list_node;
        cur = cur->next;
        if (cur->list_node == tail->list_node)
        {
            break;
        }
        
        tail->list_node->next = cur->list_node;
        tail = tail->prev;
    }

    if (cur->list_node == tail->list_node)
    {
        cur->list_node->next = NULL;
    }
    

    // printListNode(head);
    
    free_read_newNode(read_head);

    return head;
}