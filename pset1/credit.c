#include <cs50.h>
#include <math.h>
#include <stdio.h>

int numberOfDigits(long long number)
{
    if(number == 0)
        return 0;
        
    return (int)floor(log10(llabs(number))) +1;
}

int sumOfDigits(long long number)
{
    int sum = 0;
    long long val = number;
    int length = numberOfDigits(number);
    if (length == 13 || length == 15 || length == 16)
    {
        for(int i = 1; i <= length; i++)
        {
            int currentDigit = val % 10;
            if(i % 2 == 0)
            {  
                currentDigit *= 2;
                if(currentDigit > 9)
                {
                    sum += currentDigit % 10 + 1;
                } else {
                    sum += currentDigit;
                }
            } else {
                sum += currentDigit;
            }
            val /= 10;
        }
    }
    return sum;
}

string typeOfCard(long long number)
{
    int lastTwoDigits = number / (pow(10.0, numberOfDigits(number)-2) );
    switch(lastTwoDigits)
    {
        case 34:
        case 37:
            return "AMEX";
            break;
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
            return "MASTERCARD";
            break;
        default:
            if (lastTwoDigits / 10 == 4)
            {
                return "VISA";
            } else {
                return "INVALID";
            }
    }
}

int main(void)
{
    printf("Number: ");
    long long ccNumber = get_long_long();
    int sum = sumOfDigits(ccNumber);
    
    if(sum > 0 && sum % 10 == 0) {
        printf("%s\n", typeOfCard(ccNumber));
    } else {
        printf("INVALID\n");
    }
}