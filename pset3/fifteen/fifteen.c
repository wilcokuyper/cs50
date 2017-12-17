/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // calculate maximum tile value (number of tile ^ 2 - 1 (empty tile))
    int value = d * d - 1;
    
    // assign values to tiles counting down
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = value;
                
            value--;
        }
    }
    
    // If board has even number of tiles than switch tile 1 and 2
    if (d % 2 == 0)
    {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // if the tile has a 0 value than print an _, else print the number on the tile (with 2 digits)
            if(board[i][j] == 0)
                printf("  _");
            else
                printf(" %2i", board[i][j]);
        }
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == tile)
            {
                // check tile below target
                if(i < (d - 1) && board[i+1][j] == 0)
                {
                    board[i][j] = 0;
                    board[i+1][j] = tile;
                    return true;
                }
                
                // check tile above target
                if(i > 0 && board[i-1][j] == 0)
                {
                    board[i][j] = 0;
                    board[i-1][j] = tile;
                    return true;
                }
                
                // check to the right of target
                if(j < (d - 1) && board[i][j+1] == 0)
                {
                    board[i][j] = 0;
                    board[i][j+1] = tile;
                    return true;
                }
                
                // check to the left of target
                if(j > 0 && board[i][j-1] == 0)
                {
                    board[i][j] = 0;
                    board[i][j-1] = tile;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // convert 2d array to list for easy checking
    int boardValues[d*d-1];
    int counter = -1;
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            boardValues[++counter] = board[i][j];
        }
    }
    
    // check if list is in order, small to large
    int swapped = 0;
    int size = sizeof(boardValues) / sizeof(int);
    for(int i = 0; i < size-1; i++)
    {
        // if value is larger than the next one or is 0 than the list is not sorted correcly
        if(boardValues[i] > boardValues[i+1] || boardValues[i] == 0)
        {
            swapped++;
        }
    }

    // swap is 0 if all value are sorted, small to large
    return (swapped == 0);
}
