#include <iostream>
#include <algorithm>
#include <array>

using namespace std;

int main() {

    std::array<int, 10> s =
    {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
    std::array<int,10>::iterator iter;
    std::sort(s.begin(), s.end(),
    std::greater<int>());

    for(iter = s.begin(); iter != s.end(); ++iter)
        std::cout << *iter << " ";
    
    std::cout << endl;
}