#include "function_water.h"

#include <stdio.h>
#include <string.h>

#define MAXN 1000000

int len[MAXN + 5] = {0};

int main()
{
    int n, q;
    scanf("%d%d", &n, &q);

    char **string_arr = create_string_array(n);

    while (q--)
    {
        int x;
        scanf("%d", &x);

        if (x == 0)
        {
            int idx;
            char c;

            scanf("%d %c", &idx, &c);

            append_character(string_arr, len, idx, c);
        }
        else if (x == 1)
        {
            int idx;
            char str[105];

            scanf("%d%s", &idx, &str);

            append_string(string_arr, len, idx, str, strlen(str));
        }
        else if (x == 2)
        {
            print_lines(string_arr, len, n);
        }
    }

    delete_string_array(string_arr, n);
}