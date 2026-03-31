#include <stdlib.h>

char **create_string_array(int n);
void append_character(char **string_arr, int *len, int idx_line, char c);
void append_string(char **string_arr, int *len, int idx_line, char *str, int str_len);
void print_lines(char **string_arr, int *len, int n);
void delete_string_array(char **string_arr, int n);