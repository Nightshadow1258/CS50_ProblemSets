#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int question(void);
void print_block(int h);

int main()
{
    int anwser = question();
    print_block(anwser);

    printf("Hello, %d!\n", anwser);
}

int question(void)
{
    int a;
    do
    {
      printf("Heigth:\n");
      scanf("%d", &a);
    }
    while (a > 8 || a < 1);

    return a;
}

void print_block(int h)
{
  for (int i = 0; i < h; i++)
  {
      for (int j = 0; j < h-(1+i); j++)
      {
        printf(" ");
      }
      for (int j = 0; j < i+1; j++)
      {
        printf("#");
      }
      printf("  ");
            for (int j = 0; j < i+1; j++)
      {
        printf("#");
      }
            for (int j = 0; j < h-(1+i); j++)
      {
        printf(" ");
      }
      printf("\n");
  }
  
  printf("we");
}