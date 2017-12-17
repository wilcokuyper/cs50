#define _XOPEN_SOURCE
#include <unistd.h>

#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool checkPass(char *hash, char * password, char *encrypted)
{
    // compare crypt'd password to provided password
    if(strcmp(crypt(password, hash), encrypted) == 0)
    {
        printf("%s\n", password);
        return true;
    }
    return false;
}

void crack(string password)
{
    // copy hash from first 2 characters of encrypted password
    char *hash = malloc(2 * sizeof(char));
    memcpy(hash, password, 2);
    
    // allocate size for up to 4 char's)
    char *ps = malloc(4 * sizeof(char));

    // loop for 2 char's
    for (int a = 'A'; a <= 'z'; a++)
    {
        // check single character passwords
        sprintf(ps, "%c", a);
        if(checkPass(hash, ps, password))
            return;
        
        for(int b = 'A'; b <= 'z'; b++)
        {
            // check 2 character passwords
            sprintf(ps, "%c%c", a, b);
            if(checkPass(hash, ps, password))
                return;
                
            for(int c = 'A'; c <= 'z'; c++)
            {
                // check 3 character passwords
                sprintf(ps, "%c%c%c", a, b, c);
                if(checkPass(hash, ps, password))
                    return;
                    
                for(int d = 'A'; d <= 'z'; d++)
                {
                    // check 4 character passwords
                    sprintf(ps, "%c%c%c%c", a, b, c, d);
                    if(checkPass(hash, ps, password))
                        return;
                }
            }
        }
    }
    
    free(ps);
}

int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("Please specify the hashed password to crack as an argument.");
        printf("Usage: ./crack hash");
        return 1;
    }

    // crack the password!
    crack(argv[1]);

    return 0;
}