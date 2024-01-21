#include <stdio.h>
#include <string.h>
#include <ctype.h>

// constant definitions:
const int len = 1024;
char input[1024];

void encode_message(char* input, char* key);
int check_argv(char argc, char* argv[]);

int main(int argc, char* argv[])
{
    
    check_argv(argc,argv);

    printf("Enter message to encode: ");
    fgets(input,len, stdin);

    encode_message(input, argv[1]);
    printf("encoded Message: %s",input);

}

int check_argv(char argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Ussage: ./substitution key\n");
        return 1;
    }

    if ( strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }


    for (int i=0, len = strlen(argv[1]); i< len;i++)
    {
        char temp = tolower(argv[1][i]);

        if (temp >= 'a' && temp <= 'z') // check if character
        {
            for (int j=0; j < i; j++) // check if character already exists
            {
                if (temp == tolower(argv[1][j]))
                {
                    printf("%c appears twice in the key. Ensure that each char is only onces in the key\n",temp);    
                    return 1;
                }
            }
        }

    }
}

void encode_message(char* input, char* key)
{
    for (int i=0,length = strlen(input); i<length ; i++ )
    {
        char temp = input[i];
        if (temp >= 'a' && temp <= 'z')
        {
            int index = (int) temp - 'a';
            input[i] = tolower(key[index]);
        }
        if (temp >= 'A' && temp <= 'Z')
        {
            int index = (int) temp - 'A';
            input[i] = toupper(key[index]);
        }
    }
    return;
}


