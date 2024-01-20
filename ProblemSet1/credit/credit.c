#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long question(void);
void check_card(long card);

int main()
{
    long anwser = question();
    check_card(anwser);
}


long question(void)
{
    long a;
    do
    {
      printf("Enter Credit Card number please:\n");
      scanf("%ld", &a);
    }
    while ( a < 0 || a > 9999999999999999);
                         
    return a;
}


void check_card(long card)
{
    long temp = card/10;
    long first = 0;
    for (; temp > 0; temp = temp /100) // get every even digit 
    {
        long even = temp % 10;
        long evenx2 = 2 * even;
        
        if (evenx2 >= 10)
        {
            first = first + ((evenx2 / 10) % 10);
        }
        first = first + evenx2 % 10;
        printf("even: %li\n", evenx2);
    }
    printf("sum1: %li\n", first);

    printf("\n");
    temp = card;
    long second = 0;
    for (; temp > 0; temp = temp /100) // get every odd digit 
    {
        long odd = temp % 10;
        second = second + odd;
        printf("odd: %li\n", odd);
    }
    printf("sum2: %li\n", second);
    

    if ((first + second)%10 == 0)
    {
        printf("VALID\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
