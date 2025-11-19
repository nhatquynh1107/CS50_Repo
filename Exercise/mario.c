#include <cs50.h>
#include <stdio.h>
int n;
int get_positive_int(void){
    do {
        n = get_int("Height: ");
        } while ( n < 1);
        return n;
}
int main (void){
int n = get_positive_int();
for (int i = 0; i < n; i ++){
    for ( int j = 0; j < n; j++){
        if (j < n - i - 1){
            printf(" ");
        }
        else{
        printf("#");
        }
    }
    printf("\n");
}
}