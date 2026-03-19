#include <stdio.h>
#include <string.h>

int main(void){
    char str[200];
    char num1[100], num2[100], result[101];
    int plus_pos, num1_pos, num2_pos, result_pos = 0, carry = 0, num1_digit, num2_digit, sum;

    scanf("%s", &str);
    //printf("\n\n%s", str);
    
    //printf("\n\n%d", strcspn(str, "+"));
    plus_pos = strcspn(str, "+");
    strncpy(num1, str, plus_pos); // syntax: strncpy(destination, source (start copy), num(length))
    num1[plus_pos] = '\0'; // Null-terminate the substring
    //printf("\n\n%s", num1);
    num1_pos = strlen(num1) - 1;
    //printf("\n\n%d", num1_pos);
    
    strncpy(num2, str + plus_pos + 1, strlen(str) - plus_pos - 1);
    num2[strlen(str) - plus_pos - 1] = '\0'; // Null-terminate the substring
    //printf("\n\n%s", num2);
    num2_pos = strlen(num2) - 1;
    //printf("\n\n%d", num2_pos);

    while (num1_pos >= 0 || num2_pos >= 0)
    {
        if (num1_pos < 0)
        {
            num1_digit = 0; // If num1 has no more digits, treat it as 0
        }
        else
        {
            num1_digit = num1[num1_pos] - '0'; // Convert char to int by subtracting '0' (ASCII value of '0' is 48)
        }

        if (num2_pos < 0)
        {
            num2_digit = 0; // If num2 has no more digits, treat it as 0
        }
        else
        {
            num2_digit = num2[num2_pos] - '0';
        }
        sum = num1_digit + num2_digit + carry;
        //printf("\n\n%d+%d=%d", num1_digit, num2_digit, sum);

        if (sum >= 10)
        {
            carry = 1;
            sum -= 10;
        }
        else
        {
            carry = 0;
        }

        result[result_pos] = sum + '0';// Convert int back to char by adding '0'
        
        //printf("\n\n%s", result);

        num1_pos--;
        num2_pos--;
        result_pos++;
        if (num1_pos < 0 && num2_pos < 0 && carry == 1)
        {
            result[result_pos] = '1';
            result_pos++;
        }
    }

    int i = 0;
    char temp;
    // Reverse the result string
    for (i = 0; i < result_pos / 2; i++)
    {        
        temp = result[i];
        result[i] = result[result_pos - 1 - i];
        result[result_pos - 1 - i] = temp;
    }
    result[result_pos] = '\0'; // Null-terminate the result string
    printf("%s", result);

    return 0;
}