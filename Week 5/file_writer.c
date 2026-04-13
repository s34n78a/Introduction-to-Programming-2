#include <stdio.h>

int main(void)
{
    FILE *output;
    int i=10000;
    output=fopen("test.bin","wb");
    fwrite( &i, sizeof(int), 1, output );
    fclose(output);
}