#include <cs50.h>
#include <stdio.h>

void printPyramide(int height);
void printLine(int row, int height);
void printChar(string c, int count);

int main(void)
{
    int h = -1;
    while(h < 0 || h > 23) {
        printf("Height: ");
        h = get_int();
    }
    
    printPyramide(h);
}

void printPyramide(int height)
{
    for(int row = 1; row <= height; row++)
    {
        printLine(row, height);
    }
}

void printLine(int row, int height)
{
    int spaces = height - row;
    printChar(" ", spaces);
    printChar("#", row);
    printChar(" ", 2);
    printChar("#", row);
    printf("\n");
}

void printChar(string c, int count)
{
    for(int i = 0; i < count; i++)
    {
        printf("%s", c);
    }
}