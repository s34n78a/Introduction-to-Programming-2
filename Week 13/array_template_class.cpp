#include <iostream>

using namespace std;

template <class T> 
class Array {
    int size; T *data;

    public:
    T getElementAt(int);
    void setElementAt(int, T);

    Array(int);
    ~Array();
};

int main(){

    Array<int> A(3);
    for(int i=0; i<3; i++)
        A.setElementAt(i, 2*(i+1));

    for(int i=0; i<3; i++)
        cout<< A.getElementAt(i)<< " ";

    Array<string> B(2);
    B.setElementAt(0, "First Element");
    B.setElementAt(1, "Second Element");
    cout<< B.getElementAt(0) << endl;
    cout<< B.getElementAt(1) << endl;
}

template <class T>
Array<T>::Array(int s) {
    size = s;
    data = new T[size];
}

template <class T>
Array<T>::~Array() {
    delete [] data; 
}

template <class T>
T Array<T>::getElementAt(int index) {
    if (index >= 0 && index < size)
        return data[index];
    else {
        cout << "Index out of bounds" << endl;
        return T(); // Return default value of T
    }
}

template <class T>
void Array<T>::setElementAt(int index, T value) {
    if (index >= 0 && index < size)
        data[index] = value;
    else
        cout << "Index out of bounds" << endl;
}