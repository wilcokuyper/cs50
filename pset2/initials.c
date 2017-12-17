#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // ask for name input
    string name = get_string();
    char lastChar = ' ';
    
    // loop over each character in the name
    for(int i = 0; i < strlen(name); i++)
    {
        // check if name[i] is the first letter of a word (comes after a space)
        if(lastChar == ' ' && name[i] != ' ')
        {
            // print initial (uppercase)
            printf("%c", toupper(name[i]));
        }
        lastChar = name[i];
    }
    printf("\n");
}