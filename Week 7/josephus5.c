#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }

    int highest_power_of_2 = 1;
    while (highest_power_of_2 * 2 <= n) {
        highest_power_of_2 *= 2;
    }
    int survivor = 2 * (n - highest_power_of_2) + 1;
    printf("%d\n", survivor);

    return 0;
}