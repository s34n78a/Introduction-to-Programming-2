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
}

// Forward the itarator by pre-increament
template <typename T>
typename linked_list<T>::iterator linked_list<T>::iterator::operator++()
{
    // TODO:
}
// Merge two sorted linked list internally
template <typename T>
typename linked_list<T>::node *linked_list<T>::merge(node *lhs, node *rhs)
{
    // TODO:
}

// Don't forget thess lines; otherwise compiler would not generate reference symbols and you'll get a CE.
template class linked_list<int>;
template class linked_list<string>;

