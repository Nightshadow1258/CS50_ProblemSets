#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// constant definitions:
const int len = 1024;
char input[1024];
char input2[1024];
typedef struct
{
    char *name;
    int num_votes;
} candidate;

void vote(int input, candidate *candidates, int num_candidates);
int check_argv(char argc, char *argv[]);
void print_winner(candidate *candidates, int num_candidates);
int main(int argc, char *argv[])
{
    int num_candidates = argc - 1;
    if (check_argv(argc, argv) != 0)
        return 1;
    ;

    candidate candidates[num_candidates];

    for (int i = 0; i < num_candidates; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].num_votes = 0;
        // printf("%s has %d votes\n", candidates[i].name, candidates[i].num_votes);
    }
    printf("Number of voters: ");
    fgets(input, len, stdin);

    vote(atoi(input), candidates, num_candidates);
    print_winner(candidates,num_candidates);

}

int check_argv(char argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("Usage: ./plurality candidate1 candidate2 ....\n");
        return 1;
    }

    for (int j = 1; j < argc; j++)
        for (int i = 0, len = strlen(argv[j]); i < len; i++)
        {
            char temp = tolower(argv[1][i]);

            if (temp <= 'a' && temp >= 'z') // check if character
            {
                printf("Enter Names only\n");
                return 1;
            }
        }
    return 0;
}

void vote(int counter, candidate *candidates, int num_candidates)
{
    int i = 0;
    bool flag = true;
    while (i< counter)
    {
        printf("Vote: ");
        fgets(input2, len, stdin);
        // replace the enter key from the input array with \0
        char *p = strchr(input2, '\n');
        if (p)
            *p = '\0';
        // check if input is valid
        for (int j = 0; j < num_candidates; j++)
        {
            if (strcmp(candidates[j].name, input2) == 0)
            {
                candidates[j].num_votes++;
                printf("%s has now %d votes\n", candidates[j].name, candidates[j].num_votes);
                i++;
                flag = false;
                break;
            }
            else
                flag = true;
        }
        if (flag)
        {
            printf("Invaldid vote. (Will not be counted)\n");
        }
    }
}

void print_winner(candidate *candidates, int num_candidates)
{
    
    for (int i = 0; i < num_candidates)
    {
        
    }

}
