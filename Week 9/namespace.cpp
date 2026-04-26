#include <iostream>

namespace sean
{
    void printMessage() {
        std::cout << "Hello from the sean namespace!" << std::endl;
    }
    void sort(int *arr, size_t len) {
        for (size_t i = 0; i < len - 1; ++i) {
            for (size_t j = 0; j < len - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
    int max(int a, int b) {
        if (a > b) {
            return a;
        } else {
            return b;
        }
    }
}

void test() {
    sean::printMessage();
    int arr[] = {5, 2, 9, 1, 5, 6};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    sean::sort(arr, len);
    std::cout << "Sorted array: ";
    for (size_t i = 0; i < len; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    int a = 10, b = 20;
    std::cout << "Max of " << a << " and " << b << " is: " << sean::max(a, b) << std::endl;
}

int main() {
    test();
    return 0;
}