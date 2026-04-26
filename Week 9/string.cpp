#include <iostream>
#include <string> // in c it's string.h

using namespace std;

int main(void) {
    string str1 = "Hello, ";
    string str2 = "world!";
    string str3 = str1 + str2; // concatenate strings
    cout << str3 << endl; // Output: Hello, world!

    string str4 = "Hello, world!";
    if (str3 == str4) {
        cout << "str3 and str4 are equal" << endl;
    } else {
        cout << "str3 and str4 are not equal" << endl;
    }

    string str5 = "Hello";
    string str6;
    str6 = str5; // copy string
    if (str5 == str6) {
        cout << "str5 and str6 are equal" << endl;
    } else {
        cout << "str5 and str6 are not equal" << endl;
    }

    string str7 = "Hello, world!";
    cout << "Length of str7: " << str7.length() << endl;
    cout << "Size of str7: " << str7.size() << endl; // length and size are the same for string
    cout << "Character at index 1 of str7: " << str7[1] << endl; // Output: e
    cout << "Substring of str7 from index 7 with length 5: " << str7.substr(7, 5) << endl; // Output: world
    cout << "Find 'world' in str7: " << str7.find("world") << endl; // Output: 7
    str7 += " How are you?"; // append to string
    cout << str7 << endl; // Output: Hello, world! How are you?

    return 0;
}