#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id;
    int depth;
    int sup_id;
    struct Node* sup;
    struct Node** under;
    int under_count;
    int depth_calculated;
}Node;

void print_node(Node* node)
{
    printf("id: %d, depth: %d, sup_id: %d, under_count: %d, depth_calculated: %d\n", node->id, node->depth, node->sup_id, node->under_count, node->depth_calculated);
}

Node* create_node(int id, int depth, int sup_id,  Node* sup)
{
    Node* node = (Node*)malloc(sizeof(Node));
    //printf("create node %d\n", id);
    node->id = id;
    node->depth = depth;
    node->sup_id = sup_id;
    node->sup = sup;
    node->under = NULL;
    node->under_count = 0;
    node->depth_calculated = 0;
    //print_node(node);
    return node;
}

void depth_calculate(Node* node)
{
    if (node->under_count == 0)
    {
        return;
    }
    for (int i = 0; i < node->under_count; i++)
    {
        node->under[i]->depth = node->depth + 1;
        depth_calculate(node->under[i]);
    }
}

void depth_calculate_no_under(Node* node)
{
    if (node->sup->depth_calculated == 1)
    {
        node->depth = node->sup->depth + 1;
        node->depth_calculated = 1;
        return;
    }
    else
    {
        depth_calculate_no_under(node->sup);
        node->depth = node->sup->depth + 1;
        node->depth_calculated = 1;
        return;
    }
    
}

int common_sup_finder(Node* member_1, Node* member_2) {
    // print_node(member_1);
    // print_node(member_2);
    if (member_1->id == member_2->id)
    {
        // printf("same member\n");
        return member_1->id;
    }    
    else if (member_1->sup_id == member_2->id)
    {
        // printf("member 2 is sup\n");
        return member_2->id;
    }
    else if (member_2->sup_id == member_1->id)
    {
        // printf("member 1 is sup\n");
        return member_1->id;
    }
    else if (member_1->sup_id == member_2->sup_id)
    {
        // printf("same sup\n");
        return member_1->sup_id;
    }
    else if (member_1->depth == member_2->depth)
    {
        // printf("same depth\n");
        return common_sup_finder(member_1->sup, member_2->sup);
    }
    
    else if (member_1->depth > member_2->depth)
    {
        // printf("member 1 too low\n");
        return common_sup_finder(member_1->sup, member_2);
    }
    else if (member_1->depth < member_2->depth)
    {
        // printf("member 2 too low\n");
        return common_sup_finder(member_1, member_2->sup);
    }
}

int main(void) {
    int n = 0;
    int q = 0;
    Node* leader = create_node(1, 0, 0, NULL);
    leader->depth_calculated = 1;

    scanf("%d %d", &n, &q);
    Node** members = (Node**)malloc(n * sizeof(Node*));
    members[0] = leader;

    int sup_id = 0;
    for (int i = 0; i < n-1; i++)
    {
        scanf("%d", &sup_id);
        members[i+1] = create_node(i+2, 0, sup_id, NULL);
    }
    // printf("finish creating nodes\n");

    for (int i = 1; i < n; i++)
    {
        sup_id = members[i]->sup_id;
        members[i]->sup = members[sup_id-1];
        // printf("member %d's sup is %d\n", members[i]->id, members[i]->sup->id);

        // if (members[sup_id-1]->under == NULL)
        // {
        //     members[sup_id-1]->under = (Node**)malloc(sizeof(Node*) * n);
        // }
        
        // members[sup_id-1]->under[members[sup_id-1]->under_count] = members[i];
        members[sup_id-1]->under_count++;
        // printf("finish assigning member to supervisor\n");
    }

    //depth_calculate(leader);
    
    for (int i = n-1; i > 1; i--)
    {
        if (members[i]->depth_calculated == 0)
        {
            depth_calculate_no_under(members[i]);
        }
        
    }
    

    // for (int i = 0; i < n; i++)
    // {
    //     print_node(members[i]);
    // }
    
    int id_1 = 0;
    int id_2 = 0;
    for (int i = 0; i < q; i++) {
        scanf("%d %d", &id_1, &id_2);
        Node* member_1 = members[id_1-1];
        Node* member_2 = members[id_2-1];
        
        int common_sup = common_sup_finder(member_1, member_2);
        
        printf("%d\n", common_sup);
    }

    for (int i = 0; i < n; i++)
    {
        free(members[i]);
    }
    free(members);

    return 0;
}