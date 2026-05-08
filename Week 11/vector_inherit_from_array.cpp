#include <iostream>

class Array {
    protected: // so vectors can access these members
        int size;
        int *data;
    
    public:
        Array();
        Array(int);
        ~Array();
        void setSize(int);
        int getSize() const;
        void setElementAt(int, int);
        int getElementAt() const;
        void print() const;
}; // end class Array

class Vector : public Array {
    private:
        int capacity; // storage size

    public:
        Vector();
        Vector(int size);
        ~Vector();

        int getCapacity() const;
        void setSize(int);
        void print() const;
    
    Vector(): Array(){
        capacity = 0;
    } // end Vector constructor

    Vector(const int &size): Array(3*size){
        this->size = size;
        capacity = 3*size;
    } // end Vector constructor

}; // end class Vector

int main(void) {
    int size = 5;
    Vector v;
    v.setSize(size);
    for (int i = 0; i < size; ++i) {
        v.setElementAt(i, i + 1); // set elements to 1, 2, ..., size
    }
    v.print();
    return 0;
}

Array::Array() : size(0), data(nullptr) {}

Array::Array(int size) : size(size), data(new int[size]) {}

Array::~Array() {
    delete[] data;
}

void Array::setSize(int size) {
    this->size = size;
    delete[] data; // free old memory
    data = new int[size]; // allocate new memory
}

int Array::getSize() const {
    return size;
}

void Array::setElementAt(int index, int value) {
    if (index >= 0 && index < size) {
        data[index] = value;
    }
}

// int Array::getElementAt(int index) const {
//     if (index >= 0 && index < size) {
//         return data[index];
//     }
//     return -1; // or throw an exception
// }

void Array::print() const {
    for (int i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

Vector::~Vector() {
    // No need to delete data here, as Array's destructor will handle it
}

int Vector::getCapacity() const {
    return capacity;
}

void Vector::setSize(int size) {
    if (size > capacity) {
        // If new size exceeds capacity, we need to reallocate
        int newCapacity = 3 * size; // or some other growth strategy
        int *newData = new int[newCapacity];
        
        // Copy old data to new data
        for (int i = 0; i < this->size; ++i) {
            newData[i] = data[i];
        }
        
        delete[] data; // free old memory
        data = newData; // point to new memory
        capacity = newCapacity; // update capacity
    }
    this->size = size; // update size
}

void Vector::print() const {
    for (int i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

