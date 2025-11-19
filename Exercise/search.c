#include <cs50.h>
#include <stdio.h>
#include <string.h>
int main (void)
{
    string s[] = {"cat", "dog", "hehe"};
    string t = get_string("Word: ");
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(s[i],t) == 0)
        {
            printf("Found\n");
            return 0;
        }
    }
    printf("Not found\n");
    return 1;
}