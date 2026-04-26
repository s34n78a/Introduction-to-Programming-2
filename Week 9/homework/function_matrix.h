#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>

using namespace std;
class Matrix {
public:
    Matrix();
    // Construct an identity matrix.
    Matrix(long long N) {
        this->n = N;
        this->data = new long long *[N];
        for (long long i = 0; i < N; i++) {
            this->data[i] = new long long[N];
            for (long long j = 0; j < N; j++) {
                if (i == j) {
                    this->data[i][j] = 1;
                } else {
                    this->data[i][j] = 0;
                }
            }
        }
    }
    // Construct a matrix with the given data.
    Matrix(long long N, long long **Data) {
        this->n = N;
        this->data = new long long *[N];
        for (long long i = 0; i < N; i++) {
            this->data[i] = new long long[N];
            for (long long j = 0; j < N; j++) {
                this->data[i][j] = Data[i][j];
            }
        }
    }
    ~Matrix() {
        for (long long i = 0; i < this->n; i++) {
            delete[] this->data[i];
        }
        delete[] this->data;
    };
    // Print out the matrix.
    void print() {
        for (long long i = 0; i < this->n; i++) {
            for (long long j = 0; j < this->n; j++) {
                cout << this->data[i][j] << " ";
            }
            cout << endl;
        }
    }

    // TODO:
    void add(const Matrix &rhs);
    void subtract(const Matrix &rhs);
    void multiply(const Matrix &rhs);
    void transpose();
    void power(long long x);

private:
    long long n;
    long long **data;
};
#endif