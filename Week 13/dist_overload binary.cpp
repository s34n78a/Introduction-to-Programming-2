#include <iostream>

using namespace std;

class Distance {
    private:

    public:
        int feet;
        float inch;
        Distance() : feet(0), inch(0.0) {} // Default constructor
        Distance(int f, float i) : feet(f), inch(i) {} // Parameterized constructor

        // Overload the + operator to add two Distance objects
        Distance operator+(Distance& d2); // Call by reference
};

Distance Distance::operator+(Distance& d2) // Call by reference
{
    // Create an object to return
    Distance d3;
    // Perform addition of feet and inches
    d3.feet = this->feet + d2.feet;
    d3.inch = this->inch + d2.inch;

    // Return the resulting object
    return d3;
}

// Date &operator++(); // This will not work as we have not defined the operator++ for the Distance class

int main(void) {
    Distance d1(8, 9);
    Distance d2(10, 2);
    Distance d3;

    d3 = d1 + d2;

    cout << "d1 = " << d1.feet << " feet " << d1.inch << " inches" << endl;
    cout << "d2 = " << d2.feet << " feet " << d2.inch << " inches" << endl;
    cout << "d3 = d1 + d2 = " << d3.feet << " feet " << d3.inch << " inches" << endl;

    // d1.operator++(); // This will not work as we have not defined the operator++ for the Distance class

    return 0;
}