# include <stdio.h>

void show_parenthesis(int n){
    static int total = 0;
    if (n == 1) {
        printf("1");
        total = 1;
        return;
    }

    if (n%2 ==0) {
        printf("(");
    }
    show_parenthesis(n-1);
    if (n%2 == 0)
    {
        printf("+%d)", n);
        total = total + n;
    }
    else
    {
        printf("*%d", n);
        total = total * n;
    }
    
}
int main(void){
    int n;
    scanf("%d", &n);
    show_parenthesis(n);
    return 0;
}