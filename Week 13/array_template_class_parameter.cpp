#include <iostream>
#include <string>

using namespace std;

template <class T, int n>
class Array {
    int size;
    T data[n];

    public:
    T getElementAt(int);
    void setElementAt(int, T);

    Array();
    ~Array();
};

int main(){
    Array<int,3> A;
    for(int i=0; i<3; i++)
        A.setElementAt(i, 2*(i+1));
    for(int i=0; i<3; i++)
        cout<< A.getElementAt(i)<< " ";

    Array<string,2> B;
    B.setElementAt(0, "First Element");
    B.setElementAt(1, "Second Element");
    cout<< B.getElementAt(0) << endl;
    cout<< B.getElementAt(1) << endl;

}

template <class T, int n>
Array<T,n>::Array() {
    size = n;
}

template <class T, int n>
Array<T,n>::~Array() {
    // No dynamic memory to free
}

template <class T, int n>
T Array<T,n>::getElementAt(int index) {
    if (index >= 0 && index < size)
        return data[index];
    else {
        cout << "Index out of bounds" << endl;
        return T(); // Return default value of T
    }
}

template <class T, int n>
void Array<T,n>::setElementAt(int index, T value) {
    if (index >= 0 && index < size)
        data[index] = value;
    else
        cout << "Index out of bounds" << endl;
}