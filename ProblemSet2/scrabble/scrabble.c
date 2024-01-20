#include <stdio.h>
#include <string.h>
#include <ctype.h>

int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int calc_points(char word[30]);
void get_winner(int points1, int points2);

int main(void)
{
    char word1[30];
    char word2[30];
    // Prompt the user for two words
    printf("Player 1: ");
    scanf("%s", word1);

    printf("Player 2: ");
    scanf("%s", word2);

    int points1 = calc_points(word1);
    int points2 = calc_points(word2);
    // printf("Player 1 points: %d\n", points1);
    // printf("Player 2 points: %d\n", points2);
    get_winner(points1, points2);

}

int calc_points(char word[30])
{
    int points;

    // ensure lowercase for calcualtions

    for (int i = 0,len = strlen(word); i < len;i++)
    {
        word[i] = tolower(word[i]);
        points = points + POINTS [word[i] - 'a'];
    }
    return points;
}

void get_winner(int points1, int points2)
{
    if ( points1 > points2)
    {
        printf("Player 1 wins!\n");
        return;
    }
    else if ( points1 < points2)
    {
        printf("Player 2 wins!\n");
        return;
    }
    else
    {
        printf("Tie!\n");
    }
}
