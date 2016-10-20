//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: 2
// QUESTION: 1
//
// REMARKS: inplementation of Conway's Game of Life
//
//-----------------------------------------


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>


//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

const int MAX_SIZE = 60;
const int MAX_LINE = 80;
const int MAX_GEN  = 250;
const int MAX_STR  = 1024;

const char STR_ERR_INVALID_POINTER [MAX_STR] = "NULL pointer exception [file %d, line %d, function %d].";
const char STR_ERR_INVALID_PARAMS  [MAX_STR] = "invalid parameters [file %d, line %d, function %d].";


enum log_level
{
    LOG_DIAG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

typedef enum log_level logLevel;


#ifndef NDEBUG
const logLevel LOG_LEVEL = LOG_DIAG;
#else
const logLevel LOG_LEVEL = LOG_INFO;
#endif

enum result_code
{
    RESULT_OK,
    RESULT_ERR,
    RESULT_ERR_INVALID_POINTER,
    RESULT_ERR_INVALID_PARAMS,
    RESULT_ERR_UNEXPECTED_EOF,
    RESULT_ERR_UNIVERSE_DIMENSION,
    RESULT_ERR_UNIVERSE_STRUCTURE,
    RESULT_MAX_CODE
};

typedef enum result_code resultCode;


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


//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// output of logging messages
void logMsg (const logLevel LL, const char * message, ...);

// libe-by-line parser of file / std IO input
int parseInput(FILE *input);

// initialization of starting universe
int initGame (structGame *game, const char gameName[MAX_LINE], const enumState cells[MAX_SIZE*MAX_SIZE],
              const int size_x, const int size_y);

// start a new game
int playGame (structGame *game);

// generate the next generation of the universe
int nextGen (structField *generations, const int numNext);

// display of a life's generation to std output
int outputGen (const structField *universe, const int genNum);

// count neighbors (0 - 8) of a given cell
int countNeighbors (const structField *field, const int x, const int y, int *count);

// compare 2 fields to determine a cycle
int filedCmp (const structField *a, const structField *b);


//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

void logMsg (const logLevel LL, const char * message, ...)
{
    char buff [MAX_STR];
    va_list args;
    
    assert ( (LL >= LOG_DIAG) && (LL <= LOG_ERROR) );
    
    if ( (LL >= LOG_DIAG) && (LL <= LOG_ERROR) && (LL >= LOG_LEVEL) )
    {
        switch (LL)
        {
            case LOG_DIAG:
                fprintf (stderr, "DIAG: ");
                break;
            case LOG_INFO:
                fprintf (stderr, "INFO: ");
                break;
            case LOG_WARNING:
                fprintf (stderr, "WARNING: ");
                break;
            case LOG_ERROR:
                fprintf (stderr, "ERROR: ");
                break;
        }
        va_start (args, message);
        vsnprintf (buff, MAX_STR, message, args);
        fprintf (stderr, "%s", buff);
        va_end (args);
    }
}


int parseInput(FILE *input)
{
    resultCode result = RESULT_OK;
    char line[MAX_LINE] = "";           // a string for reading from an input file
    int lineNumFile = 0;                // number of current line in the input fie
    int lineNumGame = -2;               // number of current input line in the game being processed
    int size_x = 0;                     // width of the universe
    int size_y = 0;                     // height of the universe
    enumState cells[MAX_SIZE*MAX_SIZE]; // initial universe
    structGame game;                    // game object
    char gameName[MAX_LINE] = "";
    int x = 0;
    int END_OF_FILE = 0;
    
    
    while ((RESULT_OK == result) && (! END_OF_FILE))
    {
        END_OF_FILE = !(fgets (line, MAX_LINE, input));
        lineNumFile++;
        if (line[strlen(line)-1] == '\n')         // trim trailing LF
        {
            line[strlen(line)-1] = '\0';
        }
        if ( (!END_OF_FILE) && (strlen (line)) )  // ignore empty lines
        {
            lineNumGame++;
            switch (lineNumGame) // select between stages of loading game info
            {
                case -1:         // just the time to start loading new game
                {
                    assert(line[0] == '*');
                    if (line[0] != '*')
                    {
                        logMsg(LOG_ERROR, "game header not found [line %d]. Expected: a string starting with '*'. Found: \"%s\".\nProgram will quit.\n\n", lineNumFile, line);
                        result = RESULT_ERR_UNIVERSE_STRUCTURE;
                    }
                    else
                    {
                        strcpy (gameName, line);
                    }
                    break;
                }
                case 0:         // procede to reading dimensions of universe
                {
                    size_x = 0;
                    size_y = 0;
                    sscanf (line, "%d%d", &size_y, &size_x);
                    assert ( (size_x > 0) && (size_y > 0) && (size_x <= MAX_SIZE) && (size_y <= MAX_SIZE));  // validate dimensions
                    if ( (size_x <= 0) || (size_y <= 0) || (size_x > MAX_SIZE) || (size_y > MAX_SIZE))
                    {
                        logMsg(LOG_ERROR, "incorrect dimensions of the universe [line %d]. Expected: two space-separated integers from 1 to %d.\nProgram will quit.\n\n", lineNumFile, MAX_SIZE);
                        result = RESULT_ERR_UNIVERSE_DIMENSION;
                    }
                    break;
                }
                default:   // parse a line represnting initial state of universe
                {
                    assert (strlen(line) >= size_x);
                    if (strlen(line) < size_x)
                    {
                        logMsg(LOG_ERROR, "invaid line [line %d:%d]. Expected: a string of min. %d charcters ('X' or ' '. Found: \"%s\" (%d characters).\nProgram will quit.\n\n", lineNumFile, x, size_x, line, (int)strlen (line));
                        result = RESULT_ERR_UNIVERSE_STRUCTURE;
                    }
                    else
                    {
                        assert ( (lineNumGame >= 1) && (lineNumGame <= size_y) );
                        if ( (lineNumGame < 1) || (lineNumGame > size_y) )
                        {
                            logMsg(LOG_ERROR, "incorrect structure of the universe [line %d].\nProgram will quit.\n\n", lineNumFile);
                            result = RESULT_ERR_UNIVERSE_STRUCTURE;
                        }
                        else
                        {
                            x = 0;
                            while ( (x < size_x) && (line[x]) && (RESULT_OK == result) )
                            {         // package 2D universe into 1D array of cells
                                switch (line[x])
                                {
                                    case ' ':
                                        cells[(lineNumGame-1)*size_x + x] = STATE_DEAD;
                                        break;
                                    case 'X':
                                        cells[(lineNumGame-1)*size_x + x] = STATE_ALIVE;
                                        break;
                                    default:
                                        logMsg(LOG_ERROR, "invalid character [line %d:%d]: \"%s\".\nProgram will quit.\n\n", lineNumFile, x, line);
                                        result = RESULT_ERR_UNIVERSE_STRUCTURE;
                                }
                                x++;
                            }
                            if ( (lineNumGame == size_y) && (RESULT_OK == result) )
                            {         // finished reading universe, start game
                                initGame (&game, gameName, cells, size_x, size_y);
                                result = playGame (&game);
                                if (result != RESULT_OK)
                                {
                                    logMsg(LOG_ERROR, "failed to play the game \"%s\".\nProgram will quit.\n\n", game.name);
                                }
                                lineNumGame = -2;
                            }
                        }
                    }
                }
            }
        }
        else if (END_OF_FILE)
        {
            assert (lineNumGame == -2);
            if ( (lineNumGame != -2) )
            {
                logMsg(LOG_ERROR, "unexpected end of universe in input file [line %d]. Expected: %d more line%s of %d characters ('X' or ' ').\nProgram will quit.\n\n", lineNumFile, size_y - lineNumGame, (size_y - lineNumGame > 1)?"s":"", size_y);
                result = RESULT_ERR_UNEXPECTED_EOF;
            }
        }
        
    }
    assert ( (result >= 0) && (result < RESULT_MAX_CODE) );
    return result;
}


int initGame (structGame *game, const char gameName[MAX_LINE], const enumState cells[MAX_SIZE*MAX_SIZE],
              const int size_x, const int size_y)
{
    resultCode result = RESULT_ERR;
    int x = 0;
    int y = 0;
    
    assert ( (game != NULL) && (strlen(gameName) > 0) && (size_x <= MAX_SIZE) && (size_y <= MAX_SIZE) );
    if (!( (game != NULL) && (strlen(gameName) > 0) && (size_x <= MAX_SIZE) && (size_y <= MAX_SIZE) ))
    {
        logMsg(LOG_DIAG, STR_ERR_INVALID_PARAMS, __FILE__, __LINE__, __func__);
        result = RESULT_ERR;
    }
    else
    {
        strcpy (game->name, gameName);
        printf ("%s\n", game->name);
        
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
    }
    assert ( (result >= 0) && (result < RESULT_MAX_CODE) );
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
    
    structField generations[MAX_GEN+1];
    
    result = outputGen (&game->universe, 0);
    if (RESULT_OK == result)
    {
        generations[0] = game->universe;
        
        while ( (genNum < MAX_GEN) && (-1 == cycle) )
        {
            genNum++;
            nextGen (generations, genNum);
            i=-1;
            while ( (i < genNum-1) && (-1 == cycle) )
            {
                i++;
                if (filedCmp(&generations [genNum], &generations [i]) == 0)
                {
                    cycle = i;
                }
            }
        }
        assert ( ((cycle >= 0) && (cycle < genNum)) || ((cycle < 0) && (MAX_GEN == genNum)) );
        if ((cycle >= 0) && (cycle < genNum))
        {
            printf("Found a cycle between generation %d and generation %d\n", cycle, genNum);
        }
        else if ( (cycle < 0) && (MAX_GEN != genNum) )
        {
            logMsg(LOG_ERROR, "Error searching cycle.\n");
            result = RESULT_ERR;
        }
        
#ifndef NDEBUG
        outFirst = 1;
#else
        outFirst = (genNum > 10?genNum - 9:11);
#endif
        outLast = genNum;
        
        i = outFirst;
        while ( (i <= outLast) && (RESULT_OK == result) )
        {
            result = outputGen (&generations[i], i);
            i++;
        }
    }
    assert ( (result >= 0) && (result < RESULT_MAX_CODE) );
    return result;
}


int nextGen (structField *generations, const int numNext)
{
    resultCode result = RESULT_OK;
    int x = 0;
    int y = 0;
    int neighborCnt = 0;
    const int numPrev = numNext - 1;
    
    assert ( (generations != NULL) && (numNext > 0) );
    if (!( (generations != NULL) && (numNext > 0) ))
    {
        logMsg(LOG_DIAG, STR_ERR_INVALID_PARAMS, __FILE__, __LINE__, __func__);
        result = RESULT_ERR;
    }
    else
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
    assert ( (result >= 0) && (result < RESULT_MAX_CODE) );
    return result;
}


int outputGen (const structField *universe, const int genNum)
{
    resultCode result = RESULT_OK;
    int x = 0;
    int y = 0;
    int i = 0;
    char border[MAX_SIZE+2];
    
    assert (universe != NULL);
    if (NULL == universe)
    {
        logMsg(LOG_DIAG, STR_ERR_INVALID_POINTER, __FILE__, __LINE__, __func__);
        result = RESULT_ERR_INVALID_POINTER;
    }
    else
    {
        assert ( (genNum >= 0) && (genNum <= MAX_GEN) );
        if ( !( (genNum >= 0) && (genNum <= MAX_GEN) ) )
        {
            logMsg(LOG_DIAG, "invalid generation number: %d.", genNum);
            result = RESULT_ERR;
        }
        else
        {
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
        }
    }
    assert ( (result >= 0) && (result < RESULT_MAX_CODE) );
    return result;
}


int countNeighbors (const structField *field, const int x, const int y, int *count)
{
    resultCode result = RESULT_OK;
    
    assert ( (field != NULL) && (0 <= x < field->size_x) && (0 <= y < field->size_y) );
    if (!( (field != NULL) && (0 <= x < field->size_x) && (0 <= y < field->size_y) ))
    {
        logMsg(LOG_DIAG, STR_ERR_INVALID_PARAMS, __FILE__, __LINE__, __func__);
        result = RESULT_ERR;
    }
    else
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
    assert ( (result >= 0) && (result < RESULT_MAX_CODE) );
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
    assert ( (result >= 0) && (result < a->size_x * a->size_y - 1) );
    return result;
}


int main(int argc, const char * argv[])
{
    resultCode result = RESULT_OK;
    FILE *input = NULL;
    char inputPath[MAX_STR];          // path to the input file
    
    if (argc > 1)
    {
        strcpy (inputPath, argv[1]);
        input = fopen(inputPath, "r" );
        assert ( input != NULL );
        if ( input == NULL )
        {
            logMsg(LOG_INFO, "Usage: %s [input_file.txt]\n", argv[0]);
            result = RESULT_ERR;
        }
    }
    else
    {
        input = stdin;
    }
    
    if (RESULT_OK == result)
    {
        parseInput(input);
        if (strlen(inputPath))
        {
            fclose(input);
        }
    }
    assert ( (result >= 0) && (result < RESULT_MAX_CODE) );
    if (RESULT_OK == result)
    {
        printf ("Program finished normally.\n");
    }
    return result;
}
