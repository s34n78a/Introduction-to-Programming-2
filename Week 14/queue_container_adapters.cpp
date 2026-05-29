#include <iostream>
#include <queue>

using namespace std;

// Driver Code
int main()
{
    queue<int> gquiz;
    
    gquiz.push(10);
    gquiz.push(20);
    gquiz.push(30);
    
    cout << gquiz.size() <<endl;
    cout << gquiz.front()<<endl;
    cout << gquiz.back()<<endl;
    
    //cout << gquiz.pop(); //not valid
    cout << gquiz.front()<<endl;
    
    return 0;
}