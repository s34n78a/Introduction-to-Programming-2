#include <stdio.h>
#include <string.h>

int strcmp (const char *s1, const char *s2); // Compare two strings, return 0 if they are equal, a positive value if s1 is greater than s2, and a negative value if s1 is less than s2.
int strncmp (const char *s1, const char *s2, size_t size); // Compare at most size characters of two strings, return 0 if they are equal, a positive value if s1 is greater than s2, and a negative value if s1 is less than s2.
char * strcpy (char *restrict to, const char *restrict from); // Copy a string from one location to another.
char * strncpy (char *restrict to, const char *restrict from, size_t size); // Copy at most size characters from one string to another.
char * strcat (char *restrict to, const char *restrict from); // Concatenate two strings.

int main(void) {
    char str1[20] = "Hello";
    char str2[20] = "World";
    char str3[20];

    // Using strcmp
    if (strcmp(str1, str2) == 0) {
        printf("str1 and str2 are equal.\n");
    } else {
        printf("str1 and str2 are not equal.\n");
    }

    // Using strncpy
    strncpy(str3, str1, sizeof(str3) - 1);
    str3[sizeof(str3) - 1] = '\0'; // Ensure null-termination
    printf("Copied string: %s\n", str3);

    // Using strcat
    strcat(str3, " ");
    strcat(str3, str2);
    printf("Concatenated string: %s\n", str3);

    return 0;
}