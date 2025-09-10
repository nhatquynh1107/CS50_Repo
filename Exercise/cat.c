#include <cs50.h>
#include <stdio.h>
int a;
void meow(int n){
    for (int i = 0; i < n; i++){
        printf("meow\n");
    }
}
int get_positive_int(void){
    do {
        a = get_int("number: ");
        } while ( a < 1);
        return a;
}

int main(void){
    // int i = get_int("Insert ");
    // while (i > 0){
    //     printf("Meow\n");
    //     i--;
    // }
    int a = get_positive_int();
    meow(a);
}