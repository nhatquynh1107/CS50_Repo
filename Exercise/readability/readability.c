#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
const int maxN = 1e6 + 1;
int lter = 0, stence = 0, word = 1;
int main (void){
    string text = get_string("Text: ");
    for (int i = 0, n = strlen(text); i < n; i++){
        char c = text[i];
        if (isalpha(c)) 
            lter++;
        if (c == '.' || c == '?' || c == '!') 
            stence++;
        if (c == ' ')
            word++;
    }
    double L = (double)lter / word * 100.000;
    double S = (double)stence / word * 100.000;
    double idx = 0.0588 * L - 0.296 * S - 15.8;
    int ans = (int)(idx + 0.5);
    if (ans < 1)
        printf("Before Grade 1\n");
    else if (ans >= 16)
        printf("Grade 16+\n");
    else 
        printf("Grade %i\n", ans);
}