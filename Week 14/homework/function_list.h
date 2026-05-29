#ifndef FUNCTION_H
#define FUNCTION_H

#include <algorithm>
#include <string>
#include <utility>

using std::string;
#if __cplusplus >= 201402L
using std::exchange;
#else
template<typename T, typename U = T>
T exchange(T &lhs, U &&rhs)
{
    T y = lhs;
    lhs = rhs;
    return y;
}
#endif

/**
 * Declarations
 */

template <typename T>
class linked_list
{
private:
    struct node
    {
        T val;
        node *next;
        ~node();
    } *head, *tail;

    void sort(node *&);

    // TODO:
    node *merge(node *, node *); // Merge two sorted linked list internally

public:
    class iterator
    {
        friend class linked_list;

    private:
        node *n;
        iterator(node *_n);

    public:
        bool operator==(iterator) const;
        bool operator!=(iterator) const;

        // TODO:
        T &operator*();        // Dereference the iterator
        iterator operator++(); // Forward the itarator by pre-increment
    };

    linked_list();
    ~linked_list();
    iterator begin() const;
    iterator end() const;
    void push_front(T);
    void push_back(T);
    void sort();
};

/**
 * Below we have implemented most of the member functions for you!
 */

template <typename T>
linked_list<T>::node::~node()
{
    if (next)
        delete next;
}

template <typename T>
linked_list<T>::iterator::iterator(node *_n) : n(_n) {}

template <typename T>
bool linked_list<T>::iterator::operator==(iterator x) const { return n == x.n; }

template <typename T>
bool linked_list<T>::iterator::operator!=(iterator x) const { return n != x.n; }

template <typename T>
linked_list<T>::linked_list() : head(nullptr), tail(nullptr) {}

template <typename T>
linked_list<T>::~linked_list()
{
    if (head)
        delete head;
}

template <typename T>
typename linked_list<T>::iterator linked_list<T>::begin() const { return iterator(head); }

template <typename T>
typename linked_list<T>::iterator linked_list<T>::end() const { return iterator(tail->next); } // We use `nullptr` to represent the end of the list

template <typename T>
void linked_list<T>::push_front(T x)
{
    head = new node{x, head};
    if (!tail) // Empty?
        tail = head;
}

template <typename T>
void linked_list<T>::push_back(T x)
{
    if (!head) // Empty?
        head = tail = new node{x, nullptr};
    else
        tail = tail->next = new node{x, nullptr};
}

template <typename T>
void linked_list<T>::sort(node *&head)
{
    if (!head || !head->next) // Length <= 1
        return;
    node *mid = head;
    for (node *ptr = head->next; ptr;)
        if (ptr = ptr->next)
        {
            ptr = ptr->next;
            mid = mid->next;
        }
    mid = exchange(mid->next, nullptr);
    sort(head);
    sort(mid);
    head = merge(head, mid); // It's thanks to your `merge()` that enable us to make the sorting work!
}

template <typename T>
void linked_list<T>::sort()
{
    sort(head);
    for (tail = head; tail->next; tail = tail->next);
}

#endif // FUNCTION_H

////////////////////////////////////////////////////////////////////////////////
//                              Separate Line!!!                              //
////////////////////////////////////////////////////////////////////////////////

/**
 *
 * It's recommended that you should implement the 3 member functions in a different file.
 *
 * If you edit `function.h` directly, don't paste the codes above.
 *
 */

// Starter codes are provided below. Uncomment them and begin coding!

/**

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

 */
