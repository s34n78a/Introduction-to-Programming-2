#include <iostream>
#include <iomanip>

using namespace std;

class PhoneNumber
{
    friend ostream &operator<<( ostream &, const PhoneNumber & );
    friend istream &operator>>( istream &, PhoneNumber & );
    
    private:
        string areaCode; // 3-digit area code
        string exchange; // 3-digit exchange
        string line; // 4-digit line
        
}; // end class PhoneNumber

ostream &operator<<( ostream &output, const PhoneNumber &number )
{
output << "(" << number.areaCode << ") "
<< number.exchange << "-" << number.line;
return output; // enables cout << a << b << c;
}

istream &operator>>( istream &input, PhoneNumber &number )
{
    input.ignore(); // skip (
    input >> setw( 3 ) >> number.areaCode; // input area code
    input.ignore( 2 ); // skip ) and space
    input >> setw( 3 ) >> number.exchange; // input exchange
    input.ignore(); // skip dash (-)
    input >> setw( 4 ) >> number.line; // input line
    return input; // enables cin
}

int main(void) {
    PhoneNumber phone; // create object phone
    cout << "Enter phone number in the form (123) 456-7890:" << endl;
    // cin >> phone invokes operator>> by implicitly issuing
    // the non-member function call operator>>( cin, phone )
    cin >> phone;
    cout << "The phone number entered was: ";
    // cout << phone invokes operator<< by implicitly issuing
    // the non-member function call operator<<( cout, phone )
    cout << phone << endl;
}