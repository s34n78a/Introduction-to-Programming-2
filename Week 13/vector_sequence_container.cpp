#include <iostream>
#include <vector> //vector class-template

using namespace std;

int main() {
    vector<int> v;
    // add integers at the end of the vector
    v.push_back( 2 );
    v.push_back( 3 );
    v.push_back( 4 );
    cout << "\nThe size of v is: " << v.size()
    << "\nThe capacity of v is: "
    << v.capacity() << endl;

    // display the content of v
    vector<int>::const_iterator it;
    for (it = v.begin(); it != v.end(); it++)
    {
        cout << *it << endl;
    }
    return 0;
}