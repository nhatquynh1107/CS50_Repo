#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
bool only_digit(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false; 
        }
    }
     return true;
}
int main (int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    if (!only_digit(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    int key = atoi(argv[1]) % 26;
    string s = get_string("plaintext:  ");
    printf("ciphertext: ");
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        char c = s[i];
        if (isupper(c))
            printf("%c", (c - 'A' + key) % 26 + 'A');
        else if (islower(c))
             printf("%c", (c - 'a' + key) % 26 + 'a');
        else
            printf("%c", c);
    }
    printf("\n");
    return 0;
}