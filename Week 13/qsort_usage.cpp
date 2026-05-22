#include <iostream>
#include <algorithm>

using namespace std;

int compare_int (const void *a, const void *b){
    const int *va = (const int *) a;
    const int *vb = (const int *) b;
    return *va-*vb;
}

int compare_double(const void *a, const void *b){
    const double *da = (const double *) a;
    const double *db = (const double *) b;
    return (*da > *db) - (*da < *db);
}

#define SIZE 5
int data1[SIZE] = {5, 2, 9, 1, 3};
double data2[SIZE] = {3.14, 2.71, 1.41, 0.577, 1.618};

int main(void) {
    qsort(data1, SIZE, sizeof(int), compare_int);

    qsort(data2, SIZE, sizeof(double), compare_double);

    cout << "Sorted integers: ";
    for (int i = 0; i < SIZE; i++) {
        cout << data1[i] << " ";
    }
    cout << endl;

    cout << "Sorted doubles: ";
    for (int i = 0; i < SIZE; i++) {
        cout << data2[i] << " ";
    }
    cout << endl;

    return 0;
}