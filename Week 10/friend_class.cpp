#include <iostream>

class Node1 {
    private:
        int key;
        Node1* next;

        friend class LinkedList1;
};

class LinkedList1 {
    private:
        int key;
        Node1* next;
        // class Node1;
};

class Node2 {
    private:
        int key;
        Node2* next;

        friend class LinkedList2;
};

class LinkedList2 {
    private:
        int key;
        Node2* next;
        // class Node2;
};

int main() {
    LinkedList1 list1;
    LinkedList2 list2;
}