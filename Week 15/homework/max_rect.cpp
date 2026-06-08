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
        vector<long long> a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];

        long long max_area = 0;
        vector<int> st;
        st.reserve(n + 1);

        for (int i = 0; i <= n; ++i) {
            long long cur_height = (i == n ? 0 : a[i]);

            while (!st.empty() && a[st.back()] > cur_height) {
                long long height = a[st.back()];
                st.pop_back();

                int left_bound = st.empty() ? 0 : st.back() + 1;
                long long width = i - left_bound;
                max_area = max(max_area, height * width);
            }

            st.push_back(i);
        }

        cout << max_area << endl;
    }
    
    return 0;
}