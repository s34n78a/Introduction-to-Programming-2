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

void Array_MAX_HEAP::PUSH(const int &value) { //root node is stored at index 1.
    if (Count == 0)
    {
        // root node
        array[1] = value;
        Count++;
        return;
    }
    
    Count++;
    array[Count] = value;

    int child_idx = Count;
    int parent_idx = child_idx / 2;

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
    root = NULL;
}

void List_MAX_HEAP::PUSH(const int &value) {
    if (Count == 0) {
        root = new ListNode(value);
        Count++;
        return;
    }
    
    Count++;
    ListNode *parent = findparent(Count, root);
    ListNode *newNode = new ListNode(value);
    
    // Determine if this is left or right child
    if (Count % 2 == 0) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    newNode->parent = parent;
    
    // Bubble up
    ListNode *current = newNode;
    while (current->parent != NULL && current->value > current->parent->value) {
        swap(current->value, current->parent->value);
        current = current->parent;
    }
}

int List_MAX_HEAP::MAX() const {
    if (Count == 0) {
        return -1;
    }
    return root->value;
}

int List_MAX_HEAP::POP() {
    if (Count == 0) {
        return -1; // empty heap
    }
    
    int result = root->value;
    
    if (Count == 1) {
        delete root;
        root = NULL;
        Count = 0;
        return result;
    }
    
    ListNode *lastParent = findparent(Count, root);
    ListNode *lastNode;
    
    if (Count % 2 == 0) {
        lastNode = lastParent->left;
        lastParent->left = NULL;
    } else {
        lastNode = lastParent->right;
        lastParent->right = NULL;
    }
    
    root->value = lastNode->value;
    delete lastNode;
    Count--;
    
    // Bubble down
    ListNode *current = root;
    while (true) {
        ListNode *larger = current;
        
        if (current->left != NULL && current->left->value > larger->value) {
            larger = current->left;
        }
        if (current->right != NULL && current->right->value > larger->value) {
            larger = current->right;
        }
        
        if (larger == current) break;
        
        swap(current->value, larger->value);
        current = larger;
    }
    
    return result;
}

void List_MAX_HEAP::deleteTree(ListNode *root) {
    if (root == NULL) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
