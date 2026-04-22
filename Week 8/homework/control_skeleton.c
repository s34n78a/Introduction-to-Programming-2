#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* start;
    int* to;
} ChildrenCSR;

int compute_log(int n)
{
    int log = 0;
    while ((1 << log) <= n)
    {
        log++;
    }
    return log;
}

void build_child_count(int n, int* parent, int* child_count)
{
    for (int i = 1; i <= n; i++)
    {
        child_count[i] = 0;
    }

    for (int v = 2; v <= n; v++)
    {
        child_count[parent[v]]++;
    }
}

void build_children_csr(int n, int* parent, int* child_count, ChildrenCSR* children)
{
    children->start = (int*)malloc((n + 2) * sizeof(int));
    children->to = (int*)malloc((n - 1) * sizeof(int));

    children->start[1] = 0;
    for (int i = 2; i <= n + 1; i++)
    {
        children->start[i] = children->start[i - 1] + child_count[i - 1];
    }

    int* write_pos = (int*)malloc((n + 2) * sizeof(int));
    for (int i = 1; i <= n + 1; i++)
    {
        write_pos[i] = children->start[i];
    }

    for (int v = 2; v <= n; v++)
    {
        int p = parent[v];
        children->to[write_pos[p]] = v;
        write_pos[p]++;
    }

    free(write_pos);
}

void build_depth_and_up0_bfs(int n, ChildrenCSR* children, int* depth, int* up0)
{
    int* queue = (int*)malloc((n + 1) * sizeof(int));
    int head = 0;
    int tail = 0;

    queue[tail++] = 1;
    depth[1] = 0;
    up0[1] = 1;

    while (head < tail)
    {
        int u = queue[head++];
        for (int i = children->start[u]; i < children->start[u + 1]; i++)
        {
            int v = children->to[i];
            depth[v] = depth[u] + 1;
            up0[v] = u;
            queue[tail++] = v;
        }
    }

    free(queue);
}

void build_jump_table(int n, int log, int** up)
{
    for (int k = 1; k < log; k++)
    {
        for (int v = 1; v <= n; v++)
        {
            up[k][v] = up[k - 1][up[k - 1][v]];
        }
    }
}

int lift_node(int node, int dist, int log, int** up)
{
    for (int k = 0; k < log; k++)
    {
        if (dist & (1 << k))
        {
            node = up[k][node];
        }
    }
    return node;
}

int lca_query(int a, int b, int log, int* depth, int** up)
{
    if (depth[a] < depth[b])
    {
        int temp = a;
        a = b;
        b = temp;
    }

    a = lift_node(a, depth[a] - depth[b], log, up);
    if (a == b)
    {
        return a;
    }

    for (int k = log - 1; k >= 0; k--)
    {
        if (up[k][a] != up[k][b])
        {
            a = up[k][a];
            b = up[k][b];
        }
    }

    return up[0][a];
}

int main(void)
{
    int n = 0;
    int q = 0;
    scanf("%d %d", &n, &q);

    int* parent = (int*)malloc((n + 1) * sizeof(int));
    int* child_count = (int*)malloc((n + 1) * sizeof(int));
    int* depth = (int*)malloc((n + 1) * sizeof(int));

    parent[1] = 1;
    for (int i = 0; i < n - 1; i++)
    {
        scanf("%d", &parent[i + 2]);
    }

    build_child_count(n, parent, child_count);

    ChildrenCSR children;
    build_children_csr(n, parent, child_count, &children);

    int log = compute_log(n);

    int** up = (int**)malloc(log * sizeof(int*));
    for (int k = 0; k < log; k++)
    {
        up[k] = (int*)malloc((n + 1) * sizeof(int));
    }

    build_depth_and_up0_bfs(n, &children, depth, up[0]);
    build_jump_table(n, log, up);

    int id_1 = 0;
    int id_2 = 0;
    for (int i = 0; i < q; i++)
    {
        scanf("%d %d", &id_1, &id_2);
        int common_sup = lca_query(id_1, id_2, log, depth, up);
        printf("%d\n", common_sup);
    }

    for (int k = 0; k < log; k++)
    {
        free(up[k]);
    }
    free(up);
    free(children.start);
    free(children.to);
    free(depth);
    free(child_count);
    free(parent);

    return 0;
}