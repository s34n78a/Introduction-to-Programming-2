#include <iostream>

using namespace std;

template<class T, int Size>
void print(T t){
    cout<<t<<":"<<Size<<endl;
}

template <class T>
T sum(T a, T b) {
    return a+b;
}

int sum(int a, int b) {
    return a*b;
}

int main(void) {
    int data=100;
    print<int,32767>(data);
    print<int,1>(data);

    int x=7, y=15;
    cout << sum<int>(x, y) << endl; // Outputs 22
    /* compiler internally generates
    int sum(int a, int b) {
`       return a+b;
    }
    */

    double xd=7.15, yd=15.54;
    cout << sum<double>(xd, yd) << endl; // Outputs 22.69
    /* compiler internally generates
    double sum(double a, double b) {
`       return a+b;
    }
    */

    // skip <xxx>
    cout << sum(x, y) << endl; // Outputs 105 since sum(int, int) is a better match than sum<T>(T, T) for the call sum(x, y)
    cout << sum(xd, yd) << endl; // Outputs 22.69

    return 0;
}