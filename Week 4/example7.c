#include <stdio.h>

void D2B(int x){
    if(x <= 1)
        printf("%d",x);
    else{
        D2B(x/2);
        printf("%d",x%2);
    }
}

int main(void){
    int x = 13, n = 0;
    int i, bin[n];
    for(i=0;x>0;i++){
        bin[i] = x%2;
        x = x/2;
    }
    for(i--;i>=0;i--)
        printf("%d ",bin[i]);
    D2B(13);
    return 0;
}