#include <stdio.h>

void count(void) {
    static int c = 1;
    printf("%d\n", c);
    c++;
}

int main(void) {
    for(int i = 0; i < 10; i++)
        count();
    return 0;
}