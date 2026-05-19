#include <iostream>

using namespace std;

template <class T> 
int compare(const T&v1,const T& v2){
    if(v1<v2) return -1;
    return v2<v1;
}

template <class T>
int sum(const T& v1,const T& v2){
    return (v2 + v1);
}

int main(){
    int a = 2, b = 4;
    char c = 'a', d = 'Z';

    cout << compare<int>(a, b) << endl;
    cout << compare<char>(c, d) << endl;

    a = 0;
    b = 1;
    c = '0';
    d = '1';
    cout << sum<int>(a, b) << endl;
    cout << sum<char>(c, d) << endl;
    
    return 0;
}