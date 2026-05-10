#include <iostream>

struct Node
{
    int val;
    Node* left;
    Node* right;
};

void print_arr(int arr[], int n) {
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int insert_arr(int arr[], int n, int val, int &size) {
    if (arr[0] == 0)
    {
        arr[0] = val;
        size++;
        // std::cout << "first member" << std::endl;
        // print_arr(arr, n);
        return 0;
    }

    // std::cout << "insert: " << val <<  "at index " << size << std::endl;

    arr[size] = val;

    for (int i = size; i > 0; i--)
    {
        // std::cout << "i: " << i << std::endl;
        if (arr[i] < arr[i-1])
        {
            // std::cout << "swap: " << arr[i] << " and " << arr[i-1] << std::endl;
            int temp = arr[i];
            arr[i] = arr[i-1];
            arr[i-1] = temp;
        }
        else if (arr[i] > arr[i-1])
        {
            size ++;
            return i;
            break;
        }
    }
    size++;
    return 0;
    // print_arr(arr, n);
}

int pop_arr(int arr[], int n) {
    int val = arr[0];
    for (int i = 0; i < n-1; i++)
    {
        arr[i] = arr[i+1];
    }
    arr[n-1] = 0;
    return val;
}

Node* pop_node(Node* nodes[], int n) {
    Node* node = nodes[0];
    for (int i = 0; i < n-1; i++)
    {
        nodes[i] = nodes[i+1];
    }
    nodes[n-1] = NULL;
    return node;
}

int calc_total(Node* root) {
    if (root->left == NULL && root->right == NULL)
    {
        return 0;
    }
    return root->val + calc_total(root->left) + calc_total(root->right);
}

int main(void) {
    int n;
    std::cin >> n;

    Node* root = new Node();

    int arr[n] = {0};
    int val;
    int size = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> val;
        insert_arr(arr, n, val, size);
    }

    // print_arr(arr, n);

    int total = 0;
    int i = 0;
    int node_pos = -1;
    int is_node[n] = {0};
    Node* nodes[n];
    while (size > 1) {
        // std::cout << "iteration: " << i << std::endl;
        // std::cout << "arr: ";
        // print_arr(arr, n);
        // std::cout << "node_pos: " << node_pos << std::endl;
        // std::cout << "is_node: ";
        // print_arr(is_node, n);
        // std::cout << "size: " << size << std::endl;

        Node* left = new Node();
        Node* right = new Node();

        if (pop_arr(is_node, n) == 1)
        {
            // std::cout << "left is node" << std::endl;
            left = pop_node(nodes, n);
            pop_arr(arr, n);
        }
        else
        {
            // std::cout << "left is leaf" << std::endl;
            left->val = pop_arr(arr, n);
            left->left = NULL;
            left->right = NULL;
            node_pos--;
            pop_node(nodes, n);
        }
        size--;

        // std::cout << "left: " << left->val << std::endl;
        
        if (pop_arr(is_node, n) == 1)
        {
            // std::cout << "right is node" << std::endl;
            right = pop_node(nodes, n);
            pop_arr(arr, n);
        }
        else    
        {
            // std::cout << "right is leaf" << std::endl;
            right->val = pop_arr(arr, n);
            right->left = NULL;
            right->right = NULL;
            node_pos--;
            pop_node(nodes, n);
        }
        size--;

        // std::cout << "left: " << left->val << ", right: " << right->val << std::endl;
        Node* root = new Node();
        root->val = left->val + right->val;
        root->left = left;
        root->right = right;

        node_pos = insert_arr(arr, n, root->val, size);
        nodes[node_pos] = root;
        is_node[node_pos] = 1;

        // std::cout << "arr: ";
        // print_arr(arr, n);
        // std::cout << "node_pos: " << node_pos << std::endl;
        // std::cout << "is_node: ";
        // print_arr(is_node, n);
        // std::cout << "size: " << size << std::endl;
        i++;
    }

    std::cout << calc_total(nodes[0]);
    return 0;
}