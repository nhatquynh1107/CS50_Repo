#include <cs50.h>
#include <stdio.h>
const int maxN = 1e6 + 1;
const int N = 3;
float average(int length, int array[]){
    int sum = 0;
    for(int i = 0; i < length; i++){
        sum += array[i];
    }
    return sum / (float) length;
}
int main (void){
    int s[N];
    for (int i = 0; i < 3; i++)
    s[i] = get_int(" ");
    printf("%f\n", average(N, s));
}
