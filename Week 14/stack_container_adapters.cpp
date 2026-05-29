#include <iostream>
#include <stack>
#include <vector>

using namespace std;
int main() {
    stack<int, vector<int>> stack;
    
    stack.push(21);
    
    cout << stack.size();
    // cout << stack.capacity(); // Can we use function in vector? No, because stack does not have capacity function.
    
    stack.pop();

    stack.push(21);
    stack.push(22);
    stack.push(24);
    stack.push(25);
    
    stack.pop();
    stack.pop();
    
    while (!stack.empty()) {
        cout << ' ' << stack.top();
        stack.pop();
    }
}