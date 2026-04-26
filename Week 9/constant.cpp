#include <iostream>

using namespace std;

int accu1(int *arr,int size);
int accu2(const int *arr,int size);

int main(void) {
    int i =0;
    i=10;
    const int j = 0;
    // j=10; //error: j is a constant integer
    
    int *p;
    int *const q = &i; // q is a constant pointer to an integer
    // p = &i; // ok: p is a pointer to an integer
    // p = &j; // ok: p is a pointer to an integer
    // q = &j; // error: q is a constant pointer, cannot change the address it points to
    *q = 20; // ok: we can change the value

    const int *r = &i; // r is a pointer to a constant integer
    // *r = 30; // error: r points to a constant integer, cannot change the value through r
    r = &j; // ok: we can change where r points to
    // *r = 30; // error: we cannot change the value through r since it points to a constant integer

    const int *const s = &i; // s is a constant pointer to a constant integer
    // *s = 40; // error: s points to a constant integer, cannot change the value through s
    // s = &j; // error: s is a constant pointer, cannot change where s points to

    int a[3]{1,2,3};
    
    int sum1(accu1(a,3));
    cout << sum1 << endl; 

    const int sum2(accu2(a,3));
    cout << sum2 << endl;
    return 0;
}

int accu1(int *arr,int size)
{
    int sum(0);
    for(int *end = arr+size; arr!=end; ++arr)
        sum+=*arr;
    return sum;
}

int accu2(const int *arr,int size)
{
    int sum(0);
    for(const int *end = arr+size; arr!=end; ++arr)
        sum+=*arr;
    return sum;
}