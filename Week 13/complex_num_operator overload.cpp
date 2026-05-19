#include <iostream>

using namespace std;

class complex{
    double r, i;
    public:
        /* Two ways to define binary operators*/
        /* 1. a non-static member function with 1 arg.*/
        complex operator+ (const complex&);
        /* 2. a non-member function with 2 args.*/
        friend complex operator*(const complex&, const complex&);

        friend std::ostream &operator<< (std::ostream &, const complex &);
        friend std::istream &operator>> (std::istream &, complex &);

        complex(double r = 0.0, double i = 0.0) : r(r), i(i) {}
};

complex complex::operator+(const complex & c){
    return complex(c.r + this->r, c.i + this->i);
}

complex operator*(const complex& a, const complex& b){
    return complex (a.r*b.r-a.i*b.i, a.r*b.i+a.i*b.r);
}

istream& operator>> (istream &input, complex& c){
    char p; char i; //input format is a+bi
    input >> c.r >> p >> c.i >> i;
    return input;
}

ostream& operator<< (ostream &output, const complex& c){
    output << "(" << c.r << "+" << c.i << "i)";
    return output;
}

int main(void) {
    complex x, y;

    cout << "Enter a complex number (a+bi) : " << endl;
    cin >> x;

    cout << "Enter a complex number (a+bi) : " << endl;
    cin >> y;
    cout << x << " + " << y << " = " << x+y << endl;
    cout << x << " * " << y << " = " << x*y << endl;

    return 0;
}