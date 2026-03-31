#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "function_water.h"

char **create_string_array(int n){
    char **string_arr = (char**)malloc(n*sizeof(char*));
    for (int i = 0; i < n; i++)
    {
        string_arr[i] = NULL;
    }
    
    return string_arr;
}

void append_character(char **string_arr, int *len, int idx_line, char c){
    if (len[idx_line] == 0)
    {
        string_arr[idx_line] = (char*)malloc(16*sizeof(char));
    }

    else if (len[idx_line] >= 16 && len[idx_line] % 16 == 0)
    {
        string_arr[idx_line] = (char*)realloc(string_arr[idx_line], len[idx_line]*2*sizeof(char));
    }
    
    
    string_arr[idx_line][len[idx_line]] = c;
    len[idx_line] += 1;
}

void append_string(char **string_arr, int *len, int idx_line, char *str, int str_len){
    for (int i = 0; i < str_len; i++)
    {
        append_character(string_arr, len, idx_line, str[i]);
    }
    
}

void print_lines(char **string_arr, int *len, int n){
    for (int i = 0; i < n; i++)
    {
        if (len[i] > 0)
        {
            for (int j = 0; j < len[i]; j++)
            {
                printf("%c",string_arr[i][j]);
            }
            printf("\n");
        }
        
    }
    
}

void delete_string_array(char **string_arr, int n){
    for (int i = 0; i < n; i++)
    {
        free(string_arr[i]);
    }
    free(string_arr); 
}