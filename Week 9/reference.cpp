#include <iostream>

typedef struct
{
    void displayMessage() {
        std::cout << "Hello from the Shape struct!" << std::endl;
    }
} Shape;


int main() {
    int x = 10;
    int &ref = x; // ref is a reference to x
    std::cout << "Value of x: " << x << std::endl; // Output: 10
    std::cout << "Value of ref: " << ref << std::endl; // Output: 10
    std::cout << "Address of x: " << &x << std::endl; // Output: Address of x
    std::cout << "Address of ref: " << &ref << std::endl; // Output: Address of x (same as x)

    ref = 20; // Modifying ref will modify x
    std::cout << "Value of x after modifying ref: " << x << std::endl; // Output: 20
    std::cout << "Value of ref after modifying ref: " << ref << std::endl; // Output: 20

    Shape shape1;
    Shape &shapeRef = shape1; // shapeRef is a reference to shape1
    Shape *shapePtr = &shape1; // shapePtr is a pointer to shape1
    shape1.displayMessage();
    shapeRef.displayMessage(); // Accessing through reference
    shapePtr->displayMessage(); // Accessing through pointer
    
    return 0;
}