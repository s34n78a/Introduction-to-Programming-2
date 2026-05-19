#include <iostream>

class Line {
    public:
        int getLength( void );
        Line( int len ); // simple constructor
        Line( const Line &obj); // copy constructor
        ~Line(); // destructor

    private:
        int *ptr;
};

Line::Line(const Line &obj) {
    ptr = new int;
    *ptr = *obj.ptr; // copy the value
}

int main(void) {
    Line line1(10);
    Line line2 = line1; // this will invoke copy constructor

    std::cout << "Length of line1: " << line1.getLength() << std::endl;
    std::cout << "Length of line2: " << line2.getLength() << std::endl;

    return 0;
}

Line::Line(int len) {
    ptr = new int;
    *ptr = len;
}

Line::~Line() {
    delete ptr;
}

int Line::getLength( void ) {
    return *ptr;
}