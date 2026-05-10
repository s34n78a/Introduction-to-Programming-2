#include <iostream>

void sift_up(long long heap[], int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap[parent] <= heap[idx]) {
            break;
        }
        long long tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

void sift_down(long long heap[], int size, int idx) {
    while (true) {
        int left = idx * 2 + 1;
        int right = idx * 2 + 2;
        int smallest = idx;

        if (left < size && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < size && heap[right] < heap[smallest]) {
            smallest = right;
        }
        if (smallest == idx) {
            break;
        }

        long long tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    long long* heap = new long long[n * 2 + 5];
    int size = 0;

    for (int i = 0; i < n; ++i) {
        long long x;
        std::cin >> x;
        heap[size] = x;
        sift_up(heap, size);
        ++size;
    }

    long long total = 0;

    while (size > 1) {
        long long a = heap[0];
        heap[0] = heap[size - 1];
        --size;
        sift_down(heap, size, 0);

        long long b = heap[0];
        heap[0] = heap[size - 1];
        --size;
        sift_down(heap, size, 0);

        long long s = a + b;
        total += s;

        heap[size] = s;
        sift_up(heap, size);
        ++size;
    }

    std::cout << total;

    delete[] heap;
    return 0;
}
