#include <iostream>
#include "function_matrix.h"

using namespace std;

void Matrix::add(const Matrix &rhs) {
    for (long long i = 0; i < this->n; i++)
    {
        for (long long j = 0; j < this->n; j++)
        {
            this->data[i][j] += rhs.data[i][j];
        }
    }
    // this->print();
}

void Matrix::subtract(const Matrix &rhs) {
    for (long long i = 0; i < this->n; i++)
    {
        for (long long j = 0; j < this->n; j++)
        {
            this->data[i][j] -= rhs.data[i][j];
        }
    }
    // this->print();
}

void Matrix::multiply(const Matrix &rhs) {
    long long sum = 0;
    Matrix temp = Matrix(this->n, this->data);
    Matrix result = Matrix(this->n);

    for (long long i = 0; i < this->n; i++)
    {
        for (long long j = 0; j < this->n; j++)
        {
            sum = 0;
            for (long long k = 0; k < this->n; k++)
            {
                sum += temp.data[i][k] * rhs.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }

    for (long long i = 0; i < this->n; i++)
    {
        for (long long j = 0; j < this->n; j++)
        {
            this->data[i][j] = result.data[i][j];
        }
    }

    // this->print();
}

void Matrix::transpose() {
    for (long long i = 0; i < this->n; i++)
    {
        long long temp = 0;
        for (long long j = i; j < this->n; j++)
        {
            // std::cout << "swap " << i << " " << j << std::endl;
            temp = this->data[i][j];
            this->data[i][j] = this->data[j][i];
            this->data[j][i] = temp;
        }
    }
    // this->print();
}

void Matrix::power(long long x) {
    if (x == 1)
    {
        return;
    }

    Matrix base = Matrix(this->n, this->data);

    for (long long i = 0; i < this->n; i++)
    {
        for (long long j = 0; j < this->n; j++)
        {
            if (i == j)
            {
                this->data[i][j] = 1;
            }
            else
            {
                this->data[i][j] = 0;
            }
        }
    }
    
    // base.print();

    while (x > 0)
    {
        if (x%2 == 1)
        {
            this->multiply(base);
        }
        base.multiply(base);
        x /= 2;
    }
    // this->print();
}