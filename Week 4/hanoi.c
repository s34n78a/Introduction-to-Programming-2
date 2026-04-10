#include <stdio.h>
void hanoi(char a, char b, char c, int n);

int main(void){
    int n;
    printf("Number of disks: ");
    scanf("%d", &n);
    hanoi('1', '3', '2', n);
    return 0;
}
/*move n disks from a to b using c as a buffer*/
void hanoi(char a, char b, char c, int n){
    if (n == 1) {
        printf("Move disk from %c to %c\n", a, b);
        return;
    }
    hanoi(a, c, b, n-1);
    printf("Move disk from %c to %c\n", a, b);
    hanoi(c, b, a, n-1);
}