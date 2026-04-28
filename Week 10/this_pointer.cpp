#include <iostream>
#include <string.h>

using namespace std;

class Test
{
    public:
        Test( int = 0 );
        void print() const;
    private:
        int x;
};

Test::Test( int value ) {
    x = value;
}

void Test::print() const {
    // implicitly use this pointer to access the member x
    cout << "x = " << x;

    // explicitly use this pointer and the arrow operator to access the member x
    cout << "\n this->x = " << this->x;

    // explicitly use the dereferenced this pointer and the dot operator to access the member x
    cout << "\n (*this).x = " << (*this).x; // error: cannot dereference 'this->x' which is of type 'int'
}

int main()
{
    // instantiate and initialize testObject
    Test testObject(25);
    testObject.print();
}