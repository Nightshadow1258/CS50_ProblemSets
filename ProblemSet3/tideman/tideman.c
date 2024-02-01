#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define buf_size 20

#define MAX 9
// constant definitions:
char input_buf[buf_size];

// Max number of candidates


// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];


// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
char candidates[MAX][buf_size];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;


int check_argv(char argc, char *argv[]);

// Function prototypes
bool vote(int rank, char name[buf_size], int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// debug functions:
void print_ranks(int ranks[]);;
void print_preferenced(int preference[MAX][MAX]);


int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

// Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i], argv[i+1]);
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;

    int voter_count;
    printf("Number of voters: ");
    fgets(input_buf, buf_size, stdin); // to lazy to garantee that the input is a number.
    voter_count = atoi(input_buf);

    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char name[20];
            printf("Rank %i: ", j + 1);
            fgets(name, 20, stdin);
            char *p = strchr(name, '\n');
            if (p)
                *p = '\0';

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        print_ranks(ranks);
        record_preferences(ranks);

        printf("\n");
    }
    print_preferenced(preferences);


    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;

}



int check_argv(char argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./tideman [candidate ...]\n");
        return 1;
    }

    // Check for invalid usage


    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
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

// Update ranks given a new vote
bool vote(int rank, char name[buf_size], int ranks[])
{
    //TODO::
    // The function takes arguments rank, name, and ranks. If name is a match for the name of a valid candidate, then you should update the ranks array to indicate that the voter has the candidate as their rank preference (where 0 is the first preference, 1 is the second preference, etc.)
    // Recall that ranks[i] here represents the user’s ith preference.
    // The function should return true if the rank was successfully recorded, and false otherwise (if, for instance, name is not the name of one of the candidates).
    // You may assume that no two candidates will have the same name.

    for (int k = 0; k < MAX; k++)
        if (strcmp(name,candidates[k])==0)
        {
            //match found.
            ranks[rank] = k;
            return true;
        }
            // match found.
            
    printf("No Match found!");
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO:
    // The function is called once for each voter, and takes as argument the ranks array, (recall that ranks[i] is the voter’s ith preference, where ranks[0] is the first preference).
    // The function should update the global preferences array to add the current voter’s preferences. Recall that preferences[i][j] should represent the number of voters who prefer candidate i over candidate j.
    // You may assume that every voter will rank each of the candidates.
    // preferences[MAX][MAX]
    for (int i = 0; i < candidate_count; i++)
    {
        preferences[i][ranks[i]]++;
        
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}



void print_ranks(int ranks[])
{
    for (int i=0; i < candidate_count; i++)
    {
        printf("Rank entry [%d] has preference of %d\n", i, ranks[i]);
    }
}

void print_preferenced(int preference[MAX][MAX])
{
    for (int i=0; i < candidate_count; i++)
    {
        for ( int j=0; j < candidate_count; j++)
        {
        printf("|%d", preference[j][i]);
        }
        printf("|\n");
    }
}