#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 10005

void hex_to_text(const char hex[], char text[]) {
    int hex_len = (int)strlen(hex);
    int out_idx = 0;

    for (int i = 0; i + 1 < hex_len; i += 2) {
        unsigned int value = 0;
        sscanf(&hex[i], "%2x", &value);
        text[out_idx++] = (char)value;
    }

    text[out_idx] = '\0';
}

void s_to_new_s(const char s[], char new_s[]) {
    char s_copy[MAX_LEN] = {0};
    strncpy(s_copy, s, MAX_LEN - 1);

    char *token = strtok(s_copy, "|");
    while (token != NULL) {
        long long num = strtoll(token, NULL, 10);
        char hex_num[32] = {0};
        sprintf(hex_num, "%llX", num);
        strncat(new_s, hex_num, MAX_LEN - 1 - strlen(new_s));

        token = strtok(NULL, "|");
    }
}

int main(void) {
    char s[MAX_LEN] = {0};
    char new_s[MAX_LEN] = {0};
    char empty_s[MAX_LEN] = {0};
    int q;

    scanf("%10000s", s);
    scanf("%d", &q);

    s_to_new_s(s, new_s);

    for (int i = 0; i < q; i++) {
        char input[1000];
        scanf("%s", input);

        int id, len;

        char insert_s[MAX_LEN] = {0};
        char new_insert_s[MAX_LEN] = {0};

        if (strcmp(input, "Insert") == 0) //strcmp() outputs 0 if the two strings are the same
        {
            scanf("%d %10000s", &id, insert_s);

            s_to_new_s(insert_s, new_insert_s);

            int cur_len = (int)strlen(new_s);
            int ins_len = (int)strlen(new_insert_s);

            memmove(new_s + id + ins_len, new_s + id, (size_t)(cur_len - id + 1));
            memcpy(new_s + id, new_insert_s, (size_t)ins_len);
        }
        else if (strcmp(input, "Remove") == 0)
        {
            scanf("%d %d", &id, &len);
            int cur_len = (int)strlen(new_s);
            memmove(new_s + id, new_s + id + len, (size_t)(cur_len - (id + len) + 1));
        }
    }

    hex_to_text(new_s, empty_s);
    printf("%s\n", empty_s);

    return 0;
}