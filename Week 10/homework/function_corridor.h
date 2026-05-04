typedef struct Node {
    struct Node *next;
} Node;

/* Returns the node where the loop begins.
   If there is no loop, returns the last node before the exit. */
Node *detectCycle(Node *head);
