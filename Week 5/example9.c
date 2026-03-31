#include <stdio.h>
#include <string.h>

int main(void) {
    FILE *fin, *fout;
    fin = fopen("data_50000.txt", "r");
    fout = fopen("data_sorted.txt", "w");

    int num_data, i; 
    int data[1000];
    fscanf(fin, "%d", &num_data);
    for (i = 0; i < num_data; i++) {
        fscanf(fin, "%d", &data[i]);

    }
    for (i = 0; i < num_data; i++) {

        fprintf(fout, " %d", data[i]);

    }

    fclose(fin);
    fclose(fout);
}