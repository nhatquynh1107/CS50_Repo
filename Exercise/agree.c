#include<cs50.h>
#include<stdio.h>

int main(void){
    char ans = get_char("Do you agree? ");
    if (ans == 'y'|| ans == 'Y'){
        printf("Agree\n");
    }
    else{
        printf("Not agree\n");
    }
}