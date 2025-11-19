#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int point[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int game_score(string word){
int score = 0;
   for (int i = 0, n = strlen(word); i < n; i++ ){
    char c = word[i];
    if (isalpha(c)){
        char al = toupper(c);
        int id = al - 'A';
        score += point[id];
    }
   } 
   return score;
}

int main(void){
        string s = get_string("Player 1: ");
        string t = get_string("Player 2: ");
        if (game_score(s) > game_score(t)) 
            printf("Player 1 wins!\n ");
        else if (game_score(s) < game_score(t))
            printf("Player 2 wins!\n ");
        else 
            printf("Tie!\n");
}