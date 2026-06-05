#include "function_list.h"
#include <iostream>

using std::cout;

int main()
{
    linked_list<int> li;
    cout << "Created list\n";
    
    li.push_back(1);
    cout << "Pushed 1\n";
    li.push_back(3);
    cout << "Pushed 3\n";
    li.push_back(8);
    cout << "Pushed 8\n";
    
    cout << "Before sort\n";
    li.sort();
    cout << "After sort\n";
    
    cout << "Sorting complete\n";
    for (auto itr = li.begin(); itr != li.end(); ++itr)
        cout << *itr << '\n';
    
    return 0;
}
