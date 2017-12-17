#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void printCipher(string text, string key)
{
    // loop over each character
    for(int i = 0, k = 0, n = strlen(text); i < n; i++)
    {
        // check if the character is alphanumerc
        if(isalpha(text[i]))
        {
            // determine start index (upper or lower case)
            int startIndex;
            if(isupper(text[i]))
            {
                startIndex = 'A';
            }
            else
            {
                startIndex = 'a';
            }
            // print the cipher: startIndex + (char - startIndex + key) % 26,
            // where key corresponds to the nth alphanumeric letter in the plaintext modulus the keylength 
            printf("%c", startIndex + (text[i] - startIndex + tolower(key[k % strlen(key)]) - 'a') % 26);
            k++;
        }
        else
        {
            // non-alphanumeric, just return the character
            printf("%c", text[i]);
        }
    }
    printf("\n");
}

bool checkAlpha(string s)
{
    // loop over each character in string s to check if the string contains any non-alphanumeric characters
    for(int i = 0, n = strlen(s); i < n; i++){
        if(!isalpha(s[i]))
            return false;
    }
    return true;
}

int main(int argc, string argv[])
{
    // check if the key (k) is provided
    if(argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    
    // check if the key (k) only contains alphanumeric characters
    if(!checkAlpha(argv[1]))
    {
        printf("Usage: ./vigere k\n");
        return 1;
    }
    
    // ask for plaintext to convert
    printf("plaintext: ");
    string plain = get_string();
    
    //print ciphertext
    printf("ciphertext: ");
    printCipher(plain, argv[1]);
    
    return 0;
}