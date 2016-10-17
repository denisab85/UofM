#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const int MAX_SIZE = 64;
const int MAX_LINE = 80;
const int MAX_GEN  = 251;


enum RESULT_code
{
    RESULT_OK,
    RESULT_ERR,
    RESULT_ERR_INVALID_POINTER,
    RESULT_ERR_UNEXPECTED_EOF,
    RESULT_ERR_UNIVERSE_DIMENSION,
    RESULT_ERR_UNIVERSE_STRUCTURE
};


enum state_of_cell
{
    STATE_DEAD,
    STATE_ALIVE,
    NOT_INIT
};

typedef enum state_of_cell enumState;


struct field
{
    enumState cells[MAX_SIZE][MAX_SIZE];
    int size_x;
    int size_y;
};

typedef struct field structField;


struct game
{
    int generation;
    char name[MAX_LINE];
    structField universe;
};

typedef struct game structGame;


int outputGen (const structField *universe, const int genNum)
{
    int result = RESULT_OK;
    int x = 0;
    int y = 0;
    int i = 0;
    char border[MAX_SIZE+2];
    
    printf ("Generation %d:\n", genNum);
    border[0] = '+';
    for (i=0; i<universe->size_x; i++)
    {
        border[i+1] = '-';
    }
    border[universe->size_x+1] = '+';
    border[universe->size_x+2] = '\0';
    printf("%s\n", border);
    while (y < universe->size_y)
    {
        printf("|");
        x = 0;
        while (x < universe->size_x) {
            printf("%s", universe->cells[y][x]==STATE_ALIVE?"*":".");
            x++;
        }
        y++;
        printf("|\n");
    }
    printf("%s\n", border);
    return result;
}


int initGame (structGame *game, const enumState cells[MAX_SIZE*MAX_SIZE],
              const int size_x, const int size_y)
{
    int result = RESULT_ERR;
    int x = 0;
    int y = 0;
    
    printf ("%s", game->name);
    
    game->universe.size_x = size_x;
    game->universe.size_y = size_y;
    
    while (y < size_y)
    {
        x = 0;
        while (x < size_x) {
            game->universe.cells[y][x] = cells[size_x*y + x];
            x++;
        }
        y++;
    }
    return result;
}


int countNeighbors (const structField *field, const int x, const int y, int *count)
{
    int result = RESULT_OK;
    
    assert (field != NULL);
    if (field != NULL)
    {
        *count = 0;
        if ( (x > 0) && (field->cells [y] [x-1] == STATE_ALIVE) )
            *count = *count + 1;
        if ( (x > 0) && (y > 0) && (field->cells [y-1] [x-1] == STATE_ALIVE) )
            *count = *count + 1;
        if ( (y > 0) && (field->cells [y-1] [x] == STATE_ALIVE) )
            *count = *count + 1;
        if ( (x < field->size_x - 1) && (y > 0) && (field->cells [y-1] [x+1] == STATE_ALIVE) )
            *count = *count + 1;
        if ( (x < field->size_x - 1) && (field->cells [y] [x+1] == STATE_ALIVE) )
            *count = *count + 1;
        if ( (x < field->size_x - 1) && (y < field->size_y - 1) && (field->cells [y+1] [x+1] == STATE_ALIVE) )
            *count = *count + 1;
        if ( (y < field->size_y - 1) && (field->cells [y+1] [x] == STATE_ALIVE) )
            *count = *count + 1;
        if ( (x > 0) && (y < field->size_y - 1) && (field->cells [y+1] [x-1] == STATE_ALIVE) )
            *count = *count + 1;
    }
    else
    {
        result = RESULT_ERR_INVALID_POINTER;
    }
    return result;
}


int nextGen (structField *generations, int numNext)
{
    int result = RESULT_OK;
    int x = 0;
    int y = 0;
    int neighborCnt = 0;
    const int numPrev = numNext - 1;
    
    assert (numNext > 0);
    if (numNext > 0)
    {
        
        generations[numNext].size_x = generations[numPrev].size_x;
        generations[numNext].size_y = generations[numPrev].size_y;
        
        for (y = 0; y < generations[numPrev].size_y; y++)
        {
            for (x = 0; x < generations[numPrev].size_x; x++)
            {
                if (countNeighbors(&generations[numPrev], x, y, &neighborCnt) == RESULT_OK)
                {
                    if (neighborCnt == 2)
                    {
                        generations[numNext].cells[y][x] = generations[numPrev].cells[y][x];
                    }
                    else if (neighborCnt == 3)
                    {
                        generations[numNext].cells[y][x] = STATE_ALIVE;
                    }
                    else
                    {
                        generations[numNext].cells[y][x] = STATE_DEAD;
                    }
                };
            }
        }
    }
    return result;
}


int filedCmp (const structField *a, const structField *b)
{
    int result = 0;
    int x = 0;
    int y = 0;
    
    assert((a->size_x == b->size_x) && (a->size_y == b->size_y));
    if ((a->size_x == b->size_x) && (a->size_y == b->size_y))
    {
        while ((x < a->size_x) && (0 == result))
        {
            y = 0;
            while ((y < a->size_y) && (result == 0))
            {
                if (a->cells[y][x] != b->cells[y][x])
                {
                    result = x+(y*a->size_x);
                }
                y++;
            }
            x++;
        }
    }
    else
    {
        result = 1;
    }
    return result;
}


int playGame (structGame *game)
{
    int result = RESULT_OK;
    int genNum = 0;
    int i = 0;
    int cycle = -1;
    int outFirst = 0;
    int outLast = 0;
    
    structField generations[MAX_GEN];
    
    outputGen (&game->universe, 0);
    generations[0] = game->universe;
    
    while ( (genNum < MAX_GEN-1) && (-1 == cycle) )
    {
        genNum++;
        nextGen (generations, genNum);
        i=0;
        while ( (i < genNum) && (-1 == cycle) )
        {
            if (filedCmp(&generations [genNum], &generations [i]) == 0)
            {
                cycle = i;
            }
            i++;
        }
    }
    assert ( ((cycle >= 0) && (cycle < genNum)) || ((-1 == cycle) && (MAX_GEN == genNum)) );
    if ((cycle >= 0) && (cycle < genNum))
    {
        printf("Found a cycle between generation %d and generation %d\n", cycle, genNum);
    }
    if (genNum > 10)
        outFirst = genNum - 9;
    else
        outFirst = 0;
    outLast = genNum;
    for (i = outFirst; i <= outLast; i++)
    {
        outputGen (&generations[i], i);
    }
    return result;
}


int main(int argc, const char * argv[])
{
    //system("clear");
    int result = RESULT_OK;
    FILE *input = NULL;
    char inputPath[1024];               // path to the input file
    char line[MAX_LINE];                // a string for reading from an input file
    int lineNumFile = 1;                // number of current line in the input fie
    int lineNumGame = 1;                // number of current input line in the game being processed
    int size_x = 0;                     // width of the universe
    int size_y = 0;                     // height of the universe
    enumState cells[MAX_SIZE*MAX_SIZE];     // initial universe
    structGame game;
    int i = 0;
    int END_OF_FILE = 0;
    
    //    while (EOF != scanf("%s", line))
    //    {
    //        //strcpy(sig, line);
    //
    //        printf("%s\n", line);
    //    }
    if (argc > 1)
    {
        strcpy (inputPath, argv[1]);
    }
    else
    {
        printf("Usage: %s [template.txt] [data.txt]\n", argv[0]);
        exit(1);
    }
    
    input = fopen(inputPath, "r" );
    assert ( input != NULL );
    if ( input == NULL )
    {
        result = RESULT_ERR;
    }
    else
    {
        while ((RESULT_OK == result) && (! END_OF_FILE))
        {
            END_OF_FILE = !(fgets (line, MAX_LINE, input));
            if ( (!END_OF_FILE) && (RESULT_OK == result) )
            {
                if (line[0] == '*')       // load new game
                {
                    lineNumGame = 1;
                    strcpy (game.name, line);
                    if (fgets (line, MAX_LINE, input) != NULL) // read dimensions of the universe
                    {
                        lineNumFile++;
                        sscanf (line, "%d%d", &size_y, &size_x);
                        assert ( (size_x > 0) && (size_y > 0) && (size_x <= MAX_SIZE) && (size_y <= MAX_SIZE));  // validate dimensions
                        if ( (size_x <= 0) || (size_y <= 0) )
                        {
                            printf("Error: incorrect dimensions of the universe: width=%d, height=%d [line %d]\n", size_x, size_y, lineNumFile);
                            result = RESULT_ERR_UNIVERSE_DIMENSION;
                        }
                    }
                    else
                    {
                        printf("Error: unexpected end of file [line %d]\n", lineNumFile);
                        result = RESULT_ERR_UNEXPECTED_EOF;
                    }
                }
                else   // parse initial state of the universe
                {
                    assert ( (lineNumGame >= 0) && (lineNumGame <= size_y) );
                    if ( (lineNumGame >= 0) && (lineNumGame <= size_y) )
                    {
                        //printf("%s", line);
                        i = 0;
                        while ( (i < size_x) && (line[i]) && (RESULT_OK == result) )
                        {                      // package 2d universe into 1d array of cells
                            switch (line[i])
                            {
                                case ' ':
                                    cells[(lineNumGame-1)*size_x + i] = STATE_DEAD;
                                    break;
                                case 'X':
                                    cells[(lineNumGame-1)*size_x + i] = STATE_ALIVE;
                                    break;
                                default:
                                    printf("Error: invalid character [line %d]\n", lineNumFile);
                                    result = RESULT_ERR_UNIVERSE_STRUCTURE;
                            }
                            i++;
                        }
                        if ( (lineNumGame == size_y) && (RESULT_OK == result) ) // finished reading universe, start game
                        {
                            initGame (&game, cells, size_x, size_y);
                            playGame (&game);
                        }
                    }
                    else
                    {
                        printf("Error: incorrect structure of the universe [line %d]\n", lineNumFile);
                        result = RESULT_ERR_UNIVERSE_STRUCTURE;
                    }
                    lineNumGame++;
                }
                lineNumFile++;
            }
            else // EOF
            {
                assert (lineNumGame == size_y);
                if ( (lineNumGame != size_y) )
                {
                    printf("Error: unexpected end of universe in input file: line %d\n", lineNumFile);
                    result = RESULT_ERR_UNEXPECTED_EOF;
                }
            }
        }
    }
    
    return result;
}