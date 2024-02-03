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

// check if j or i has more candidates
int check_argv(char argc, char *argv[]);

// Function prototypes
bool vote(int rank, char name[buf_size], int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// debug functions:
void print_ranks(int ranks[]);
void print_preferences(int preference[MAX][MAX]);
void print_pairs(void);
void print_locked(void);;


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
        strcpy(candidates[i], argv[i + 1]);
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

        // Query for each// check if j or i has more candidates rank
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
        //print_ranks(ranks);
        record_preferences(ranks);

        printf("\n");
    }
    print_preferences(preferences);

    add_pairs();
    print_pairs();

    sort_pairs();
    print_pairs();

    lock_pairs();
    print_locked();

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
    // TODO::
    //  The function takes arguments rank, name, and ranks. If name is a match for the name of a valid candidate,
    // then you should update the ranks array to indicate that the voter has the candidate as their rank preference
    // (where 0 is the first preference, 1 is the second preference, etc.)
    //  Recall that ranks[i] here represents the user’s ith preference.
    //  The function should return true if the rank was successfully recorded, and false otherwise
    // (if, for instance, name is not the name of one of the candidates).
    //  You may assume tha// check if j or i has more candidatest no two candidates will have the same name.

    for (int k = 0; k < MAX; k++)
        if (strcmp(name, candidates[k]) == 0)
        {
            // match found.
            ranks[rank] = k;
            return true;
        }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO:
    // The function is called once for each voter, and takes as argument the ranks array, (recall that ranks[i] is
    // the voter’s ith preference, where ranks[0] is the first preference).
    // The function should update the global preferences array to add the current voter’s preferences. Recall that
    // preferences[i][j] should represent the number of voters who prefer candidate i over candidate j.
    // You may assume that every voter will rank each of the candidates.
    // preferences[MAX][MAX]

    // The higher ranks are winning againnst all the are below them.
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; j < candidate_count - i; j++)
            preferences[ranks[i]][ranks[i + j]]++;
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO:
    // The function should add all pairs of candidates where one candidate is preferred to the pairs array. A pair of 
    // candidates who are tied (one is not preferred over the other) should not be added to the array.
    // The function should update the global variable pair_count to be the number of pairs of candidates. 
    // (The pairs should thus all be stored between pairs[0] and pairs[pair_count - 1], inclusive).
    // initialize the pair count to 0

    // only the corner of the preference matrix have to be compared to each other this how to only iterate over them? efficently

    pair_count = 0;

    for (int i = 1; i < candidate_count; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (i == j) // ignore
                continue;
            // check if j or i has more candidates
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            else // tied -> do not add
            {
                // printf("pair %d and %d are tied with votes %d = %d\n", i,j,preferences[i][j],preferences[j][i]);
                continue;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // The function should sort the pairs array in decreasing order of strength of victory, where strength of victory is
    // defined to be the number of voters who prefer the preferred candidate. If multiple pairs have the same strength
    // of victory, you may assume that the order does not matter.

    for (int i = 0 ; i < pair_count - 1; i++)
    {
        for (int j = 0 ; j < pair_count - i - 1; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] <= preferences[pairs[j+1].winner][pairs[j+1].loser])
            {
                pair temp = pairs[j+1];
                pairs[j+1] = pairs[j];
                pairs[j] = temp;
            }
        }
     }
    
    return;

}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // The function should create the locked graph, adding all edges in decreasing order of victory strength so long as 
    // the edge would not create a cycle.
    // need to check if there is already a edge conntect. Do this by checking if there is already a value true in the row of the end node.

    // create adjacency Matrix:
    for ( int i = 0 ; i < pair_count; i++ )
    {   
        int i = pairs[i].loser;
        bool no_edge_flag = true;
        // check if row and column does not contain a true value
        for ( int j = 0; j <candidate_count; j++)
        {
            if (locked[i][j] == true && locked[j][i]) // edge found
                no_edge_flag = false;

        }
        if (no_edge_flag == true)
            locked[pairs[i].winner][pairs[i].loser] = true;
    }
    

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // The function should print out the name of the candidate who is the source of the graph. You may assume there
    //  will not be more than one source
    // find source of the locked graph. This should be the node where no other nodes lead to so the colums of the
    // locked (adjacency matrix) must be zero for all cases.

    // traverse locked matrix through each colum and check if an entry is true.
    // if entry found set flag
    int winner_ind=-1;
    bool empty_col = true;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if ( locked[j][i] == true)
                empty_col = false;
        }
        if (empty_col == true)
        {
            winner_ind = i;
            break;
        }
        empty_col = true;
    }
    if (winner_ind >= 0 && winner_ind < candidate_count)
        printf("The Winner is: %s  !!\n", candidates[winner_ind]);
    else
        printf("no winner found ERROR!\n");

    return;
}

void print_ranks(int ranks[])
{
    printf("RANKS:\n");
    for (int i = 0; i < candidate_count; i++)
    {
        printf("Rank entry [%d] has preference of %d\n", i, ranks[i]);
    }
}

void print_preferences(int preference[MAX][MAX])
{
    printf("PREFERENCES:\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("|%d", preference[i][j]);
        }
        printf("|\n");
    }
}

void print_pairs(void)
{
    printf("PAIRS:\n");

    for (int i = 0; i < candidate_count; i++)
    {
        printf("Pair[%d]: Winner is %s, Loser is %s\n", i, candidates[pairs[i].winner], candidates[pairs[i].loser]);
    }
}

void print_locked(void)
{
    printf("LOCKED:\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("|%d", locked[i][j]);
        }
        printf("|\n");
    }
}