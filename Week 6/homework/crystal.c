#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct
{
    long long value;
    long long receive;
    int height;
    int id;
} Crystal;

int main(void)
{
    int n = 0;
    if (scanf("%d", &n) != 1)
    {
        return 0;
    }

    Crystal *stack = (Crystal *)malloc((size_t)n * sizeof(Crystal));

    if (stack == NULL)
    {
        return 0;
    }

    int top = -1;
    long long max_receive = -1;
    int max_id = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        Crystal current;
        scanf("%d %lld", &current.height, &current.value);
        current.receive = 0;
        current.id = i;

        /*
         * Keep the stack strictly decreasing by height.
         * Any spire that is shorter than the current one has now found its
         * nearest taller spire on the right, which is this current spire.
         */
        while (top >= 0 && stack[top].height < current.height)
        {
            /* Current spire receives the popped spire's energy. */
            current.receive += stack[top].value;

            /* Track the best receiver seen so far. */
            if (stack[top].receive > max_receive || (stack[top].receive == max_receive && stack[top].id < max_id))
            {
                max_receive = stack[top].receive;
                max_id = stack[top].id;
            }

            top--;
        }

        if (top >= 0)
        {
            /* The remaining top is the nearest taller spire on the left. */
            stack[top].receive += current.value;
        }

        /* Store the current spire for future comparisons. */
        stack[++top] = current;
    }

    /*
     * Spires left in the stack never found a taller spire on the right,
     * so only their received energy from the left has to be checked.
     */
    while (top >= 0)
    {
        if (stack[top].receive > max_receive || (stack[top].receive == max_receive && stack[top].id < max_id))
        {
            max_receive = stack[top].receive;
            max_id = stack[top].id;
        }

        top--;
    }

    printf("%d %lld\n", max_id + 1, max_receive);

    free(stack);
    return 0;
}