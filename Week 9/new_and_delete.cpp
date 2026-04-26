#include <iostream>
#include <cstdlib> // for malloc and free

using namespace std;    

int main(void) {
    // don't forget to free the memory allocated by malloc, otherwise it will cause memory leak
    // don't mix malloc/free with new/delete, otherwise it will cause undefined behavior

    //old version
    int *p1 = (int*)malloc(sizeof(int)); //allocate an int
    if(p1==NULL) //error: malloc failed
    {
        cerr << "Memory allocation failed" << endl;
        return 1; //exit with error code
    }
    *p1 = 42; //use the allocated memory
    cout << "Value: " << *p1 << endl; //print the value
    free(p1); //deallocate the memory

    //new version
    int *p2(new int); //allocate an int
    // no need to check for null pointer, new will automatically throw an exception if allocation fails
    *p2 = 42; //use the allocated memory
    cout << "Value: " << *p2 << endl; //print the value
    delete p2; //deallocate the memory

    int *p3(new int(10));
    //allocate an int
    // and set its value 10
    int *p4 = new int(10);
    cout << "Value: " << *p3 << endl; //print the value
    cout << "Value: " << *p4 << endl; //print the value
    delete p3; //deallocate the memory
    delete p4; //deallocate the memory

    int *arr = new int[5]; //allocate an array of 5 ints
    for (int i = 0; i < 5; ++i) {
        arr[i] = i + 1; //initialize the array
    }
    cout << "Array: ";
    for (int i = 0; i < 5; ++i) {
        cout << arr[i] << " "; //print the array
    }
    cout << endl;
    delete[] arr; //deallocate the array

    int **matrix = new int*[3]; //allocate an array of 3 pointers to int
    for (int i = 0; i < 3; ++i) {
        matrix[i] = new int[4]; //allocate an array of 4 ints for each pointer
    }
    // initialize the matrix
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] = i * 4 + j + 1; // set value to i*4+j+1
        }
    }
    cout << "Matrix:" << endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << matrix[i][j] << " "; //print the matrix
        }
        cout << endl;
    }
    // deallocate the matrix
    for (int i = 0; i < 3; ++i) {
        delete[] matrix[i]; //deallocate each array of ints
    }
    delete[] matrix; //deallocate the array of pointers

    return 0;
}