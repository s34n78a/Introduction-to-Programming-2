#include <iostream>

using namespace std;

struct Edge {
    int to;
    long long weight;
    int next;
};

static void farthest_node(int n, const int* head, const Edge* edges, int start,
                          int& best_node, long long& best_dist) {
    int* parent = new int[n];
    long long* dist = new long long[n];
    int* st = new int[n];
    int top = 0;

    for (int i = 0; i < n; i++) {
        parent[i] = -1;
        dist[i] = 0;
    }

    st[top++] = start;
    parent[start] = start;

    best_node = start;
    best_dist = 0;

    while (top > 0) {
        int u = st[--top];

        if (dist[u] > best_dist) {
            best_dist = dist[u];
            best_node = u;
        }

        for (int ei = head[u]; ei != -1; ei = edges[ei].next) {
            int v = edges[ei].to;
            if (v == parent[u]) {
                continue;
            }
            parent[v] = u;
            dist[v] = dist[u] + edges[ei].weight;
            st[top++] = v;
        }
    }

    delete[] parent;
    delete[] dist;
    delete[] st;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) {
        return 0;
    }

    if (n <= 1) {
        cout << 0 << '\n';
        return 0;
    }

    int* head = new int[n];
    Edge* edges = new Edge[2 * (n - 1)];

    for (int i = 0; i < n; i++) {
        head[i] = -1;
    }

    long long total_weight = 0;
    int idx = 0;
    int start = 1;

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        edges[idx].to = v;
        edges[idx].weight = w;
        edges[idx].next = head[u];
        head[u] = idx++;

        edges[idx].to = u;
        edges[idx].weight = w;
        edges[idx].next = head[v];
        head[v] = idx++;

        total_weight += w;
        start = u;
    }

    int a = 0;
    long long dummy = 0;
    farthest_node(n, head, edges, start, a, dummy);

    int b = 0;
    long long diameter = 0;
    farthest_node(n, head, edges, a, b, diameter);

    cout << 2 * total_weight - diameter << '\n';

    delete[] head;
    delete[] edges;
    return 0;
}
