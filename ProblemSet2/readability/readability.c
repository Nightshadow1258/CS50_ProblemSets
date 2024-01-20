#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// constant definitions:
const int len = 1024;

// function declarations:
int get_char_count(char text[len]);
int get_word_count(char text[len]);
int get_sentence_count(char text[len]);
void print_debug_info(int char_count, int word_count, int sentence_count);
void calc_grade(int char_count, int word_count, int sentence_count);

int main(void)
{
    
    char input[len];

    // Prompt the user for two words
    printf("Enter Text to grade: ");
    fgets(input,len, stdin);

    int char_count = get_char_count(input);
    int word_count = get_word_count(input);
    int sentence_count = get_sentence_count(input);

    print_debug_info(char_count, word_count, sentence_count);

    calc_grade(char_count, word_count, sentence_count);
}

void print_debug_info(int char_count, int word_count, int sentence_count)
{
    printf("character count: %d\n",char_count);
    printf("word count: %d\n",word_count);
    printf("sentence count: %d\n",sentence_count);
}

int get_char_count(char text[len]) 
{
    int char_count = 0;
    for(int i=0, length = strlen(text); i<length ; i++)
    {
        int temp = tolower(text[i]);
        if ( temp >= 'a' && temp <= 'z')
        {
            char_count++;
        }   
    }
    return char_count;
}

int get_word_count(char text[len])
{
    // use spacebar as a indication for a finished word.
    int word_count = 0;
    for(int i=0, length = strlen(text); i<length ; i++)
    {
        int temp = tolower(text[i]);
        if ( temp == ' ')
        {
            word_count++;
        }   
    }
    // add one since there will be no space at the end of the text.
    // ++ as to be in front else it will increment after returning whcch is useless.
    return ++word_count;
}

int get_sentence_count(char text[len])
{
    // use ! or . or ? as a indication for a finished sentence.
    int sentence_count = 0;
    for(int i=0, length = strlen(text); i<length ; i++)
    {
        int temp = tolower(text[i]);
        if ( temp == '.' || temp == '!' || temp == '?')
        {
            sentence_count++;
        }   
    }
    return sentence_count;
}

void calc_grade(int char_count, int word_count, int sentence_count)
{
    float L = (float) char_count / word_count * 100;
    float S = (float) sentence_count / word_count * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    //rounding operation: add 0.5 if positiv subtract 0.5 if negative and truncate with cast to int
    int grade =  (int)(index < 0 ? (index - 0.5) : (index + 0.5));
    printf("L: %f\nS: %f\nINDEX: %f\nGrade: %d\n", L,S,index,grade);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
    printf("Grade %d\n", (int) grade);
    }
}
