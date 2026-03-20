#include <stdio.h>
#include "function_ex.h"
int main(){
    int a, b;
    scanf("%d%d",&a,&b);
    printf("%d\n",call_add(a,b));
    return 0;
}
