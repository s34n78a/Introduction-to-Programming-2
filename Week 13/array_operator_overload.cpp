#include <iostream>

using namespace std;

const int SIZE = 10;

class Array {
    private:
        int arr[SIZE];
    public:
        Array() {
            for (int i = 0; i < SIZE; ++i) {
                arr[i] = 0;
            }
        }
        int &operator[](int i);
        const Array &operator=( const Array & );
};

int &Array::operator[](int i) {
    if( i >= SIZE || i < 0 ) {
        cout << "Index out of bounds" <<endl; // return first element.
        return arr[0];
    }
    return arr[i]; }

const Array &Array::operator=( const Array &rhs ) {
    if (this != &rhs) {
        for (int i = 0; i < SIZE; ++i) {
            arr[i] = rhs.arr[i];
        }
    }
    return *this;
}

int main(void) {
    Array myArray;
    myArray[5] = 10;
    cout << myArray[5] << endl;

    Array C = myArray; // copy constructor
    cout << C[5] << endl;
    
    C[3] = myArray[5] + 5;
    cout << C[3] << endl;

    return 0;
}