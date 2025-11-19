#include <cs50.h>
#include <stdio.h>
int a1 = 0, a2 = 0, a3 = 0, a4 = 0, x;
int main(void)
 {
  do 
  {
    x = get_int("Change owed: ");
  } 
  while (x < 0);

  a1 = x / 25;
  x -= 25 * a1;

  a2 = x / 10;
  x -= 10 * a2;

  a3 = x / 5;
  x -= 5 * a3;

  a4 += x;
  x -= a4;
  printf("%i\n", a1 + a2 + a3 + a4);
}