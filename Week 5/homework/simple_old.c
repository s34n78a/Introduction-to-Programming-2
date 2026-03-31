#include <stdio.h>
#include <string.h>

void hex_to_text(char hex[], char text[]) {
    //printf("hex to text\n");
    char temp[3];
    int len = strlen(hex);

    for (int i = 0; i < len; i+=2)
    {
        //printf("%d\n", i);
        //printf("%d %s\n", strlen(hex), hex);
        //printf("%c%c\n",hex[i],hex[i+1]);
        
        temp[0] = hex[i];
        temp[1] = hex[i+1];
        temp[2] = '\0';
        
        //printf("%s\n", temp);
        
        sscanf(temp, "%x", &text[i/2]);
        //printf("%s\n", text);
    }
    
}

void s_to_new_s(char s[], char new_s[]) {
    long long int num;
    char base_16[] = "0123456789ABCDEF";
    
    char *token = strtok(s, "|");
    while (token != NULL) {
        //printf("%s\n", token);

        sscanf(token, "%d", &num);
        //printf("%d\n", num);

        char new_num[10000] = {0};
        new_num[0] = base_16[num%16];
        while (num != 0)
        {
            //printf("%c\n", base_16[num%16]);
            num = num/16;
            new_num[strlen(new_num)] = base_16[num%16];
        }
        new_num[strlen(new_num)] = '\0';
        //printf("%s\n", new_num);
        
        for (int i = 0; i < (strlen(new_num)-1)/2; i++)
        {
            char temp = new_num[i];
            new_num[i] = new_num[strlen(new_num)-2-i];
            new_num[strlen(new_num)-2-i] = temp;
        }
        //printf("%s\n", new_num);
        
        for (int i = 0; i < strlen(new_num)-1; i++)
        {
            new_s[strlen(new_s)] = new_num[i];
        }
        //printf("%s\n\n", new_s);

        token = strtok(NULL, "|");
    }
}

int main(void) {
    char s[10000], new_s[10000], empty_s[10000];
    char base_16[] = "0123456789ABCDEF";
    int q, id, len;

    scanf("%s", s);
    scanf("%d", &q);

    //printf("%s\n", s);
    s_to_new_s(s, new_s);
    //printf("%s\n", new_s);

    for (int i = 0; i < q; i++) {
        char input[1000];
        scanf("%s", input);
        //printf("%s\n", input);

        int id, len;

        char insert_s[10000];
        char new_insert_s[10000];

        if (strcmp(input, "Insert") == 0) //strcmp() outputs 0 if the two strings are the same
        {
            // printf("note\n");
            // printf("%s\n", insert_s);
            // printf("%s\n", new_insert_s);
            scanf("%d %s", &id, insert_s);
            
            s_to_new_s(insert_s, new_insert_s);
            // printf("%s\n", new_insert_s);

            int new_len = strlen(new_s) + strlen(new_insert_s);

            for (int i = 0; i < strlen(new_insert_s); i++)
            {
                for (int j = strlen(new_s)-1; j >= id+i; j--)
                {
                    new_s[j+1] = new_s[j];
                    // printf("%s\n", new_s);
                }
                new_s[id+i] = new_insert_s[i];
            }
            new_s[new_len] = '\0';
            
            // printf("%s\n", new_s);
            memset(insert_s, 0, sizeof(insert_s));
            memset(new_insert_s, 0, sizeof(new_insert_s));
        }
        else if (strcmp(input, "Remove") == 0)
        {
            // printf("yooo\n");

            scanf("%d %d", &id, &len);
            for (int i = 0; i < id+len-1; i++)
            {
                new_s[id+i] = new_s[id+len+i];
            }
            for (int i = id+len-1; i < strlen(new_s)-len; i++)
            {
                new_s[i] = new_s[len+i];
            }
            new_s[strlen(new_s)-len] = '\0';
            // printf("%s\n", new_s);
        }
    }
        
    hex_to_text(new_s, empty_s);
    printf("%s\n", empty_s);
    
    return 0;
}