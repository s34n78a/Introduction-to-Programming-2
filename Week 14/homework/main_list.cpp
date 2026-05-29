#include "function_list.h"

#include <iostream>

using std::cin;
using std::cout;

int main()
{
    int n, m;
    while (cin >> n >> m)
    {
        linked_list<int> li;
        while (n--)
        {
            int x;
            cin >> x;
            li.push_back(x);
        }
        linked_list<string> ls;
        while (m--)
        {
            string x;
            cin >> x;
            ls.push_front(x);
        }
        li.sort();
        for (auto itr = li.begin(); itr != li.end(); ++itr)
            cout << *itr << '\n';
        ls.sort();
        for (auto itr = ls.begin(); itr != ls.end(); ++itr)
            cout << *itr << '\n';
    }
    return 0;
}
