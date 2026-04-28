#include "function_matrix.h"
#include <iostream>
using namespace std;

signed main() {
    long long n, t;
    cin >> n >> t;
    long long **tmp = new long long *[n];
    for (long long i = 0; i < n; i++) {
        tmp[i] = new long long[n];
    }
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < n; j++) {
            cin >> tmp[i][j];
        }
    }
    Matrix res(n, tmp);
    while (t--) {
        int o;
        cin >> o;
        if (o == 1) {
            for (long long i = 0; i < n; i++) {
                for (long long j = 0; j < n; j++) {
                    cin >> tmp[i][j];
                }
            }
            Matrix mat(n, tmp);
            res.add(mat);
        } else if (o == 2) {
            for (long long i = 0; i < n; i++) {
                for (long long j = 0; j < n; j++) {
                    cin >> tmp[i][j];
                }
            }
            Matrix mat(n, tmp);
            res.subtract(mat);
        } else if (o == 3) {
            for (long long i = 0; i < n; i++) {
                for (long long j = 0; j < n; j++) {
                    cin >> tmp[i][j];
                }
            }
            Matrix mat(n, tmp);
            res.multiply(mat);
        } else if (o == 4) {
            res.transpose();
        } else if (o == 5) {
            long long x;
            cin >> x;
            res.power(x);
        }
    }
    res.print();
}