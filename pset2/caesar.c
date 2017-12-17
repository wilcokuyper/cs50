#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void printCipher(string text, int key)
{
    for(int i = 0; i < strlen(text); i++)
    {
        // check if text is alphanumeric
        if(isalpha(text[i]))
        {
            // determine start index (upper or lower case)
            int startIndex;
            if(isupper(text[i])) {
                startIndex = 'A';
            }
            else
            {
                startIndex = 'a';
            }
            // print the cipher: startIndex + (char - startIndex + key) % 26
            printf("%c", startIndex + (text[i] - startIndex + key) % 26);
        }
        else 
        {
            // non-alphanumeric, just print original character
            printf("%c", text[i]);
        }
    }
    printf("\n");
}

int main(int argc, string argv[])
{
    // check if the key (k) is provided
    if(argc != 2)
    {
        printf("Incorrect number of arguments specified!\n");
        printf("Usage: ./caesar k\n");
        return 1;
    }
    
    // ask for plaintext to convert
    printf("plaintext: ");
    string plaintext = get_string();
    
    // print ciphertext
    printf("ciphertext: ");
    printCipher(plaintext, atoi(argv[1]));
    
    return 0;
}