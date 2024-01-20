#include <stdio.h>
#include <string.h>
#include <ctype.h>

int get_char_count(char text);

int main(void)
{
    int len = 1024;
    char input[len];
    // Prompt the user for two words
    printf("Enter Text to grade: ");
    fgets(input,len, stdin);

    printf("%s",input);
}


int get_char_count(char text)
{
    for(int i=0, len = strlen(text); i<len ; i++)
    {
        
    }
}