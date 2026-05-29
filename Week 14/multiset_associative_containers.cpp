#include <iostream>
#include <set>

using namespace std;

int main()
{
    multiset< int, less< int > > ms;
    ms.insert( 10 ); // insert 10
    ms.insert( 35 ); // insert 35
    ms.insert( 10 ); // insert 10 again (allowed)
    
    cout << "There are " << ms.count( 10 ) << " entries of 10"; // returns the number of entries = 10
    
    multiset < int, less< int > >::iterator it; // creates an iterator
    
    it = ms.find(10);
    
    if ( it != ms.end() ) // iterator not at end
        cout << "\n10 was found";
    
    return 0;
}