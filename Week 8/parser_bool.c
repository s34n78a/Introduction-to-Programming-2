#include <stdio.h>
#include <stdlib.h>

int evalBoolExpr(){
    char c = getchar();
    if(c == '&' || c == '|') {
        int op1 = evalBoolExpr();
        int op2 = evalBoolExpr();
        return c == '&' ? op1 && op2 : op1 || op2;
    } else if(c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    return 0;
}

int main() {
    int a = 5, b = 10;
    if(a > b) {
        printf("a is greater than b\n");
    } else {
        printf("a is not greater than b\n");
    }
    return 0;
}