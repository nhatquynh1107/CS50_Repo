#include <cs50.h>
#include <stdio.h>
void swap (int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}