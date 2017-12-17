#include <cs50.h>
#include <stdio.h>

int calculateBottles(int min);

int main(void)
{
    printf("Minutes: ");
    int minutes = get_int();
    printf("Bottles: %i\n", calculateBottles(minutes));
}

int calculateBottles(int min)
{
    return min * 12;
}