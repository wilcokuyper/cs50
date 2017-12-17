/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    
    /* Linear Search
    for(int i = 0; i < n; i++)
    {
        if(value == values[i])
            return true;
    }
    return false;
    */
    
    // Binary Search
    int min = 0;
    int max = n-1;
    int mid = (max + min) / 2;
    
    do
    {
        if (value == values[mid])
        {
            return true;
        }
        else if(value > values[mid])
        {
            min = mid + 1;
        }
        else 
        {
            max = mid - 1;
        }
        
        mid = (max + min) / 2;
    }
    while(min <= max);
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    
    // Bubble sort
    int swap = -1;
    int j = n-1;
    do
    {
        swap = 0;

        for(int i = 0; i < j; i++)
        {
            if(values[i] > values[i+1])
            {
                int t = values[i];
                values[i] = values[i+1];
                values[i+1] = t;
                swap++;
            }
        }
        j--;
    }
    while(swap > 0);
   
    return;
}
