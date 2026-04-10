#include <stdio.h>
int main(void) {
    int input;
    while (scanf("%d", &input) == 1) {
        if (input < 0)
            input = -input;
        printf("%d\n", input);
    }
    return 0;
}