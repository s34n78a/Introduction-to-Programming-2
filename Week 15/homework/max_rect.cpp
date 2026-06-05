#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main(void) {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];
        
        int max_area = 0;
        for (int i = 0; i < n; ++i) {
            int height = a[i];
            int left = i, right = i;
            while (left > 0 && a[left - 1] >= height)
                --left;
            while (right < n - 1 && a[right + 1] >= height)
                ++right;
            max_area = max(max_area, height * (right - left + 1));
        }
        
        cout << max_area << endl;
    }
    
    return 0;
}