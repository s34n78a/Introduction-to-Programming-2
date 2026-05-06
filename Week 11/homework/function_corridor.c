#include <stdio.h>
#include <stdlib.h>
#include "function_corridor.h"

Node *detectCycle(Node *head) {
    Node* slow = head;
    Node* fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            slow = head;
            
            while (slow != fast)
            {
                slow = slow->next;
                fast = fast->next;
            }
            
            return slow;
        }
    }

    while (slow->next != NULL)
    {
        slow = slow->next;
    }
    
    return slow;
}