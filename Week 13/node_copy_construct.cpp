#include <iostream>

class ListNode {
    public:
        ListNode(int value) : value(value) {}
        int getValue() const { return value; }

    private:
        int value;
};

ListNode function () {
    ListNode A(3);
    return A;
}

void function (ListNode B) {
    std::cout << B.getValue() << std::endl;
}

int main (){
    ListNode A(3);
    function(A);

    ListNode B = A;
    function(B);
}