#include <iostream>
#include <array>
#include <algorithm>
#include <functional>

using namespace std;

int main() {
    //container adapters do not have sort function, but we can use sort function in array

    std::array<int, 10> s = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3}; // container
    
    std::array<int,10>::iterator iter; // iterator
    
    std::sort(s.begin(), s.end(), std::greater<int>()); // algorithm to sort in descending order

    for(iter = s.begin(); iter != s.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << endl;
}