#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    FILE *input;
    int i=10000;
    input=fopen("test.bin","rb");
    if ( input==NULL ) {
        printf("open input file fail!\n");
        exit(0);
    }
    fread( &i, sizeof(int), 1, input );
    printf("%d\n",i);
    fclose(input);
}