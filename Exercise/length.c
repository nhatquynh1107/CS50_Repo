#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main (void){
    string s = get_string(" ");
    printf(" ");
   for (int i = 0, n = strlen(s); i < n; i++){
    
        printf("%C", toupper(s[i]));
    
   }
   printf("\n");
}