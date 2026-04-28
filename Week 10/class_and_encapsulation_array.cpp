#include <iostream>
#include <string.h>

using namespace std;

// class definition
class Array
{
    private:
        int size; // size of the array
        int *data; // pointer to the array data

    public:
    // constructor initializes the size of
    // the array
        Array(int n) {
            // if(n>0) {
            //     size = n;
            //     data = new int[n];
            // } else {
            //     size = 0;
            //     data = nullptr;
            // }
            data = nullptr;
            setArraySize(n);
        }
        ~Array() {
            delete [] data;
            data = nullptr;
        }

    /* function that displays a welcome
    message to the Array users*/
        void displayMessage() const
        {
            cout << "Welcome!" << endl;
        } // end function displayMessage
        
        
        int getArraySize() const{
            return size;
        }

        // void setArraySize(int n) {
        //     size = n; // is that ok?
        // }
        void setArraySize(int n);

}; // end class Array

void Array::setArraySize(int n){
    if (data == nullptr) {
        // can only set size if data is NULL
        if(n>0) {
            size = n;
            data = new int[n];
        } else {
        size = 0;
        }
    } else {
        cerr << "cannot reset array size \n";
    }
} // end function

int main()
{
    Array myArray(5);
    myArray.displayMessage();
}