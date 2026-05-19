#include <iostream>

using namespace std;

class LinkedList{
    struct Node{ int data; Node *next;};
    Node *head, *tail;

public:

    friend class LLIterator;
    class LLIterator {
        LinkedList::Node *curr;
        public:
            LLIterator() ;
            LLIterator(LinkedList::Node *head);
            LLIterator(const LLIterator& it);
            ~LLIterator();
            LLIterator& operator=(const LLIterator&);
            bool operator!=(const LLIterator& it);
            LLIterator& operator++();
            int& operator*();
    };

    LinkedList();
    ~LinkedList();
    void pushBack(int in);
    LLIterator begin();
    LLIterator end();
};

int main(void) {
    LinkedList L1;

    for (int i=0; i<5; i++)
    {
        L1.pushBack(i);
    }

    LinkedList::LLIterator it;
    for (it = L1.begin(); it != L1.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;

    return 0;
}

LinkedList::LLIterator::LLIterator() : curr(nullptr) {}

LinkedList::LLIterator::LLIterator(LinkedList::Node *head) : curr(head) {}

LinkedList::LLIterator::LLIterator(const LLIterator& it) : curr(it.curr) {}

LinkedList::LLIterator::~LLIterator() {}

LinkedList::LLIterator& LinkedList::LLIterator::operator=(const LLIterator& it) {
    if (this != &it) {
        curr = it.curr;
    }
    return *this;
}

bool LinkedList::LLIterator::operator!=(const LLIterator& it) {
    return curr != it.curr;
}

LinkedList::LLIterator& LinkedList::LLIterator::operator++() {
    if (curr) {
        curr = curr->next;
    }
    return *this;
}

int& LinkedList::LLIterator::operator*() {
    if (curr) {
        return curr->data;
    }
    throw std::out_of_range("Dereferencing end iterator");
}

LinkedList::LinkedList() : head(nullptr), tail(nullptr) {}

LinkedList::~LinkedList() {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        delete current;
        current = next;
    }
}

void LinkedList::pushBack(int in) {
    Node *newNode = new Node{in, nullptr};
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

LinkedList::LLIterator LinkedList::begin() {
    return LLIterator(head);
}

LinkedList::LLIterator LinkedList::end() {
    return LLIterator(nullptr);
}

