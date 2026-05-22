#include <iostream>

using namespace std;

template <class T, class U>
T smaller(T a, U b) {
    return (a < b ? a : b);
    //The expression (a < b ? a : b) is equivalent to the expression:
    //if a is smaller than b, return a, else, return b.
}

int main () {
    int x=72;
    double y=15.34;
    cout << smaller(x, y) << endl;
}