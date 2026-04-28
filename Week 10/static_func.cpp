#include <iostream>
using namespace std;
class Array {
    private:
        static unsigned int cnt;
        // number of objects instantiated
    public:
        Array(){Array::cnt++;} // constructor
        ~Array(){Array::cnt--;} // destructor
        // static member function
        static unsigned int getCount(){
            return Array::cnt;
        }// return # of objects instantiated
}; // end class Shape

unsigned int Array::cnt = 0;

int main() {
    // use static functions without any object
    cout << "Number of arrays is " << Array::getCount() << endl;
    Array s1;
    Array s2;
    cout << "Number of arrays is " << Array::getCount() << endl;
}