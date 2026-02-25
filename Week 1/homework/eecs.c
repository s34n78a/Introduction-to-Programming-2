#include <stdio.h>
#include <ctype.h>

int main(void) {
    int k;
    char s[6];

    scanf("%d", &k);
    scanf("%5s", s);

    int shift = k % 26;

    for (size_t i = 0; i < 5; i++)
    {
        s[i] = toupper(s[i]);
        s[i] = (char)('A' + (s[i] - 'A' + shift) % 26);
    }
    
    printf("%s", s);

    return 0;
}