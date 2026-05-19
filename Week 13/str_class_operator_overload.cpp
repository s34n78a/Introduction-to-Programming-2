#include <iostream>

using namespace std;

int main(void) {
    string s1("happy" );string s2(" birthday");
    cout << "\n The results of comparing s2&s1:"
    << "\ns2==s1 yields "
    << (s2==s1 ? "true" : "false")
    << "\ns2!=s1 yields "
    << (s2!=s1 ? "true" : "false")
    << "\ns2 >s1 yields "
    << (s2 >s1 ? "true" : "false")
    << "\ns2 <s1 yields "
    << (s2 <s1 ? "true" : "false")
    << "\ns2>=s1 yields "
    << (s2>=s1 ? "true" : "false")
    << "\ns2<=s1 yields "
    << (s2<=s1 ? "true" : "false");
}

/*
class String
{
public:
String operator+( const String & ) const;
bool operator<(const String & ) const;
...
}; // end class String

if y and z are String-class objects, then y < z is treated as if
y.operator<(z) had been written.


• Unary Overloaded Operators as Member Functions

class String
{
public:
bool operator!() const;
...
}; // end class String

When the compiler sees the expression !s (in which s is an object of class String), the compiler generates the function call s.operator!().


• Unary Overloaded Operators as Non-Member Functions

bool operator!( const String & );

!s is treated as if the call operator!(s) had been written, invoking the non- member operator! function
*/