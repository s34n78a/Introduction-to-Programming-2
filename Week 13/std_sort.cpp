#include <iostream>
#include <algorithm>
#include <array>

/*
template< class RandomIt >
void sort1( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
void sort2( RandomIt first, RandomIt last,
Compare cmp );*/

// bool cmp(const Type1 &a, const Type2 &b);

int main(){

    std::array<int, 10> s1={5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

    // sort using the default operator<
    std::sort(s1.begin(), s1.end());
    for (int a : s1) {
        std::cout << a << " ";
    }
    std::cout << '\n';

    std::array<int, 10> s2={5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
    /* sort using a standard library

    compare function object*/

    std::sort(s2.begin(), s2.end(), std::greater<int>());

    for (int a : s2) {
        std::cout << a << " ";
    }
    std::cout << '\n';
}