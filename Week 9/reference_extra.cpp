#include <iostream>

// biar ga ush pake std:: di depan cout dan endl
using namespace std;

// atau bisa juga pake ini, tapi jangan dua-duanya
using std::cout; // kalau taro di local scope, maka cuma berlaku di scope itu aja
using std::endl;

int squareByValue( int number ) {
    return number *= number;
} // end function squareByValue

void squareByPointer( int *numberPtr ){
    *numberPtr *= *numberPtr;
} // end function squareByPointer

void squareByReference( int &numberRef ){
    numberRef *= numberRef;
} // end function squareByReference

int main(){
    int x = 2, y = 3, z = 4;
    // demonstrate squareByValue
    cout << "x = " << x << " before squareByValue\n";
    cout << "Value returned by squareByValue: "
    << squareByValue( x ) << endl;
    cout << "x = " << x << " after squareByValue"<< endl;
    // demonstrate squareByPointer
    cout << "y = " << y << " before squareByPointer\n";
    squareByPointer( &y );
    cout << "y = " << y << " after squareByPointer\n";
    // demonstrate squareByReference
    cout <<"z = "<< z <<" before squareByReference\n";
    squareByReference(z);
    cout <<"z = "<< z << " after squareByReference\n";
} // end main