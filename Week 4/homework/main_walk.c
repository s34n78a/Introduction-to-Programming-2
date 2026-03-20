#include <stdio.h>
#include <stdlib.h>
#include "function_walk.h"

int main()
{
    int t;
    scanf("%d", &t);

    while (t--)
    {
        int n, q;
        scanf("%d%d", &n, &q);
        int len[105];
        for (int i = 0; i < n; ++i)
        {
            scanf("%d", len + i);
        }
        int **arr = create_walkway(n, len);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < len[i]; ++j)
            {
                scanf("%d", arr[i] + j);
            }
        }
        for (int i = 0; i < q; ++i)
        {
            activate_walkway(n, len, arr);
        }

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < len[i]; ++j)
            {
                printf("%d ", arr[i][j]);
            }
            putchar('\n');
        }
        delete_walkway(n, arr);
    }
}