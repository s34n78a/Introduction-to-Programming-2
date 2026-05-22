#include <iostream>
#include <deque>

using namespace std;

void showdq(deque <int> g)
{
    deque <int> :: iterator it;
    for (it = g.begin(); it != g.end(); ++it)
        cout << '\t' << *it;
        cout << '\n';
    
    /*for (int i=0; i<g.size(); i++)
        cout << '\t' << g.at(i) ;*/
}

int main(void) {
    deque <int> gquiz;
    gquiz.push_back(10);
    gquiz.push_front(20);
    gquiz.push_back(30);
    gquiz.push_front(15);
    cout << "The deque gquiz is : ";
    showdq(gquiz);
}