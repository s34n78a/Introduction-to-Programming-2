#include "function_list.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <utility>

using std::string;
using std::cin;
using std::cout;

// Starter codes are provided below. Uncomment them and begin coding!

// Dereference the iterator
template <typename T>
T &linked_list<T>::iterator::operator*()
{
    // TODO:
    // Hint: You can access the value of the node by `n->val`.

    return n->val;
}

// Forward the itarator by pre-increament
template <typename T>
typename linked_list<T>::iterator linked_list<T>::iterator::operator++()
{
    // TODO:
    // Hint: You can access the next node by `n->next`.

    n = n->next;
    return *this;
}
// Merge two sorted linked list internally
template <typename T>
typename linked_list<T>::node *linked_list<T>::merge(node *lhs, node *rhs)
{
    // TODO:
    // Hint: You can create a dummy node to simplify the merging process. After the merging, return `dummy.next` as the head of the merged linked list.

    node dummy;
    dummy.next = nullptr;
    node *cur = &dummy;

    while (lhs && rhs)
    {
        if (lhs->val < rhs->val)
        {
            cur->next = lhs;
            lhs = lhs->next;
        }
        else  // lhs->val >= rhs->val
        {
            cur->next = rhs;
            rhs = rhs->next;
        }
        
        cur = cur->next;
    }
    
    // Attach remaining nodes
    cur->next = lhs ? lhs : rhs;
    
    node *result = dummy.next;
    dummy.next = nullptr;  // Prevent destructor from deleting the merged list
    return result;
}

// Don't forget these lines; otherwise compiler would not generate reference symbols and you'll get a CE.
template class linked_list<int>;
template class linked_list<string>;

