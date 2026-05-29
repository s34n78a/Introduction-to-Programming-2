#include <queue>
#include <iostream>

using namespace std;

// Driver Code
int main()
{
    priority_queue<int, vector<int>, greater<int>> gquiz;
    
    gquiz.push(10);
    gquiz.push(30);
    gquiz.push(20);
    
    cout << gquiz.size() <<endl;
    cout << gquiz.top()<<endl;
    
    gquiz.pop();
    
    cout << gquiz.top()<<endl;
    
    return 0;
}