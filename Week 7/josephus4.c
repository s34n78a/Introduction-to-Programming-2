#include <stdio.h>
#include <stdlib.h>

int Josephus(int n)
{

    if (n==1) return 1;
    else if (n%2 == 0)
        return 2*Josephus(n/2)-1;
    else
        return 2*Josephus(n/2)+1;

}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }
    printf("%d\n", Josephus(n));
    return 0;
}