#include <iostream>

struct Edge
{
    long long weight;
    long long from;
    long long to;
    long long next;
};

void printEdge(Edge* edge)
{
    if (edge == NULL)
    {
        return;
    }

    std::cout << "Edge from ";
    if (edge->from > -1)
    {
        std::cout << edge->from;
    }
    else
    {
        std::cout << "NULL";
    }
    
    std::cout << " to ";
    if (edge->to > -1)
    {
        std::cout << edge->to;
    }
    else
    {
        std::cout << "NULL";
    }

    std::cout << " with weight ";
    if (edge->weight > -1)
    {
        std::cout << edge->weight;
    }
    else
    {
        std::cout << "NULL";
    }
    
    std::cout << " and next edge index ";
    if (edge->next > -1)
    {
        std::cout << edge->next;
    }
    else
    {
        std::cout << "NULL";
    }
    std::cout << std::endl;
}

void farthestNode(Edge* edges, long long* source_node_next_edge, long long n, long long start, long long& best_node, long long& best_dist){
    long long* stack = new long long[n];
    long long* dist = new long long[n];
    long long* parent = new long long[n];
    long long top = 0;

    for (long long i = 0; i < n; i++)
    {
        dist[i] = 0;
        parent[i] = -1;
    }

    stack[top++] = start;
    parent[start] = start;

    best_node = start;
    best_dist = 0;

    while (top > 0)
    {
        long long cur_node = stack[--top];

        if (dist[cur_node] > best_dist)
        {
            best_dist = dist[cur_node];
            best_node = cur_node;
        }

        for (long long current_edge = source_node_next_edge[cur_node]; current_edge != -1; current_edge = edges[current_edge].next)
        {
            long long next_node = edges[current_edge].to;

            if (next_node == parent[cur_node])
            {
                // Skip the edge that leads back to the parent node
                continue;
            }

            parent[next_node] = cur_node;
            dist[next_node] = dist[cur_node] + edges[current_edge].weight;
            stack[top++] = next_node;
        }
    }
    
    delete[] stack;
    delete[] dist;
    delete[] parent;
}

int main(void) {
    long long n;
    long long total_weight = 0;
    std::cin >> n;

    Edge* edges = new Edge[2*(n-1)];
    long long* source_node_next_edge = new long long[n];

    for (long long i = 0; i < n; i++)
    {
        source_node_next_edge[i] = -1;
    }

    long long from, to, weight;
    long long edge_index = 0;

    for (long long i = 0; i < n-1; i++)
    {
        std::cin >> from >> to >> weight;
        
        edges[edge_index].from = from;
        edges[edge_index].to = to;
        edges[edge_index].weight = weight;

        // here we can set the next pointer to -1 or to the index of the next edge in the adjacency list
        edges[edge_index].next = source_node_next_edge[from];
        source_node_next_edge[from] = edge_index;
        edge_index++;

        edges[edge_index].to = from;
        edges[edge_index].from = to;
        edges[edge_index].weight = weight;
        
        edges[edge_index].next = source_node_next_edge[to];
        source_node_next_edge[to] = edge_index;
        edge_index++;

        total_weight += weight;
    }

    // for (long long i = 0; i < 2*(n-1); i++)
    // {
    //     printEdge(&edges[i]);
    // }

    // std::cout << "Total weight: " << total_weight << std::endl;
    
    long long start_node = from; // You can choose any node as the starting point
    long long best_node, best_dist;

    farthestNode(edges, source_node_next_edge, n, start_node, best_node, best_dist);

    farthestNode(edges, source_node_next_edge, n, best_node, best_node, best_dist);

    std::cout << 2*total_weight - best_dist << std::endl;

    delete[] edges;
    delete[] source_node_next_edge;
    return 0;
}