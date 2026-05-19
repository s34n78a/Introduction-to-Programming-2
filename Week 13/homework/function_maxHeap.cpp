#include"function_maxHeap.h"
#include<iostream>
using namespace std;

Array_MAX_HEAP::Array_MAX_HEAP() {
    for (int i = 0; i < 1025; i++)
    {
        array[i] = -1;
    }
    
    Count = 0;
}

Array_MAX_HEAP::~Array_MAX_HEAP() {
    delete[] array;
}

void Array_MAX_HEAP::PUSH(const int &value) { //root node is stored at index 1.
    if (Count == 0)
    {
        // root node
        array[1] = value;
        Count++;
    }
    
    array[Count + 1] = value;
    Count++;

    int parent_idx = (Count) / 2;
    int child_idx = Count;

    while (parent_idx > 0 && array[parent_idx] < array[child_idx])
    {
        swap(array[parent_idx], array[child_idx]);
        child_idx = parent_idx;
        parent_idx = child_idx / 2;
    }
}

int Array_MAX_HEAP::MAX() const {
    return array[1];
}

int Array_MAX_HEAP::POP() {
    if (Count == 0)
    {
        return -1; // empty heap
    }
    

    int return_value = array[1];

    array[1] = array[Count];
    array[Count] = -1;
    Count--;

    int parent_idx = 1;
    int l_child_idx = 2;
    int r_child_idx = 3;

    while (l_child_idx <= Count)
    {
        int max_child_idx = l_child_idx;

        if (r_child_idx <= Count && array[r_child_idx] > array[l_child_idx])
        {
            max_child_idx = r_child_idx;
        }

        if (array[parent_idx] < array[max_child_idx])
        {
            swap(array[parent_idx], array[max_child_idx]);
            parent_idx = max_child_idx;
            l_child_idx = parent_idx * 2;
            r_child_idx = parent_idx * 2 + 1;
        }
        else
        {
            break;
        }
    }

    return return_value;
}


List_MAX_HEAP::List_MAX_HEAP() {
    root = nullptr;
}

void List_MAX_HEAP::PUSH(const int &) {

}

int List_MAX_HEAP::MAX() const {
    return 0;
}

int List_MAX_HEAP::POP() {
    if (Count == 0)
    {
        return -1; // empty heap
    }

    return 0;
}

void List_MAX_HEAP::deleteTree(ListNode *root) {
    if (root == nullptr)
    {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
