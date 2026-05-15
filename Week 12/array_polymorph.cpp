#include <iostream>

class Array {
    int size;
    int *data;
    public:
    Array();
    Array(int);
    ~Array();
    // void setSize(int);
    virtual void setSize(int, int); // virtual function
    int getSize() const;
    void setElementAt(int, int);
    int getElementAt() const;
    // void print() const;
    virtual void print() const; // virtual function
};

class Vector : public Array {
    private:
        int capacity; // storage size

    public:
        Vector();
        Vector(int size);
        ~Vector();

        int getCapacity() const;
        // void setSize(int); 
        virtual void setSize(int, int) override; // override base class setSize
        // void print() const; 
        virtual void print() const override; // override base class print
};

int main(void) {
    Array a; // base-class object
    Vector v; // derived-class object
    Array *arrayPtr; // base-class pointer
    Vector *vectorPtr; // derived-class pointer

    a.setSize(5, 0); // set size of array to 5
    v.setSize(10, 20); // set size of vector to 10 and capacity to 20

    vectorPtr = &v;
    // vectorPtr = &a; // error: cannot convert from Array* to Vector*
    vectorPtr->print(); // call vector print

    /* aim base-class pointer at derived-class object and print */
    arrayPtr = &v; // point to v (vector object)

    /* polymorphism; invokes Vector's print*/
    arrayPtr ->print(); // call array print

    arrayPtr = &a;
    /* polymorphism; invokes Array's print*/
    arrayPtr->print();

    // If Vector’s destructor is not virtual
    Array *pa = new Vector(3);
    delete pa;
    // C++ standard specifies that the behavior is undefined

    return 0;
}

Array::Array() {
    size = 0;
    data = nullptr;
}

Array::Array(int s) {
    size = s;
    data = new int[size];
}

Array::~Array() {
    delete [] data;
}

void Array::setSize(int s, int c) {
    size = s;
    data = new int[size];
}

int Array::getSize() const {
    return size;
}

void Array::setElementAt(int index, int value) {
    if (index >= 0 && index < size) {
        data[index] = value;
    }
}

int Array::getElementAt() const {
    return data[0];
}

void Array::print() const {
    std::cout << "Array of size " << size << std::endl;
}

Vector::Vector() : Array() {
    capacity = 0;
}

Vector::Vector(int s) : Array(s) {
    capacity = s;
}

Vector::~Vector() {
    // base class destructor is automatically called
}

int Vector::getCapacity() const {
    return capacity;
}

void Vector::setSize(int s, int c) {
    Array::setSize(s, c); // call base class setSize
    capacity = c; // set capacity to the provided value
}

void Vector::print() const {
    std::cout << "Vector of size " << getSize() << " and capacity " << capacity << std::endl;
}

