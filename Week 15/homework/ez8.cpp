#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <string>
#include <climits>
#include <vector>

using namespace std;

int main(void) {
    int t;
    int arr[3][3];
    int solution[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    cin >> t;
    
    // neighbor indices for the 3x3 grid (0-based)
    vector<vector<int>> neigh = {
        {1,3}, {0,2,4}, {1,5},
        {0,4,6}, {1,3,5,7}, {2,4,8},
        {3,7}, {4,6,8}, {5,7}
    };

    string target = "123456780";

    while (t--) {
        string start;
        start.reserve(9);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int x; cin >> x;
                start.push_back(char('0' + x));
            }
        }

        if (start == target) {
            cout << "You can solve it within 0 steps." << '\n';
            continue;
        }

        // bidirectional BFS
        unordered_map<string,int> d1, d2;
        queue<string> q1, q2;
        d1[start] = 0; d2[target] = 0;
        q1.push(start); q2.push(target);

        int answer = -1;

        auto expand = [&](queue<string> &q, unordered_map<string,int> &dcur, unordered_map<string,int> &dother)->bool {
            int sz = q.size();
            while (sz--) {
                string cur = q.front(); q.pop();
                int dist = dcur[cur];
                if (dist >= 14) continue; // no need to expand beyond limit
                int zpos = cur.find('0');
                for (int nb : neigh[zpos]) {
                    string nxt = cur;
                    swap(nxt[zpos], nxt[nb]);
                    if (!dcur.count(nxt)) {
                        dcur[nxt] = dist + 1;
                        if (dother.count(nxt)) {
                            answer = dcur[nxt] + dother[nxt];
                            return true;
                        }
                        q.push(nxt);
                    }
                }
            }
            return false;
        };

        // run until either meet or exhausted or answer >14
        bool found = false;
        while (!q1.empty() && !q2.empty()) {
            // expand smaller frontier
            if (q1.size() <= q2.size()) {
                if (expand(q1, d1, d2)) { found = true; break; }
            } else {
                if (expand(q2, d2, d1)) { found = true; break; }
            }
            // early cutoff if minimal possible already >14
            int min1 = INT_MAX, min2 = INT_MAX;
            for (auto &p : d1) min1 = min(min1, p.second);
            for (auto &p : d2) min2 = min(min2, p.second);
            if (min1 + min2 > 14) break;
        }

        if (found && answer != -1 && answer <= 14) {
            cout << "You can solve it within " << answer << " steps." << '\n';
        } else {
            cout << "You'd better skip this game." << '\n';
        }
    }
    return 0;
}