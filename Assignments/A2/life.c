#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


const int MAX_SIZE = 60;
const int MAX_LINE = 1024;
const int MAX_GEN  = 250;


enum RESULT_code
{
    RESULT_OK,
    RESULT_ERR,
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

typedef enum state_of_cell State;


struct field
{
    State cells[MAX_SIZE][MAX_SIZE];
    int size_x;
    int size_y;
};

typedef struct field Field;


struct game
{
    int generation;
    char name[MAX_LINE];
    Field universe;
};

typedef struct game Game;


int equal (Field a, Field b)
{
    int result = RESULT_ERR;
    int x = 0;
    int y = 0;
    
    assert((a.size_x == b.size_x) && (a.size_y == b.size_y));
    if ((a.size_x == b.size_x) && (a.size_y == b.size_y))
    {
        while ((x < a.size_x) && (1 == result))
        {
            y = 0;
            while ((y < a.size_y) && (1 == result))
            {
                if (a.cells[x][y] != b.cells[x][y])
                {
                    result = RESULT_OK;
                }
                y++;
            }
            x++;
        }
    }
    else
    {
        result = RESULT_OK;
    }
    return result;
}

int output (Field *universe, int generation)
{
    int result = RESULT_OK;
    int x = 0;
    int y = 0;
    int i = 0;
    char border[MAX_SIZE+2];
    
    printf ("Generation %d:\n", generation);
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
            printf("%s", universe->cells[y][x]==STATE_ALIVE?"*":" ");
            x++;
        }
        y++;
        printf("|\n");
    }
    printf("%s\n", border);
    return result;
}

int init (Game *game, State cells[MAX_SIZE*MAX_SIZE], int size_x, int size_y)
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

int play (Game *game)
{
    int result = RESULT_OK;
    int generation = 0;
    
    output (&game->universe, generation);
    
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
    State cells[MAX_SIZE*MAX_SIZE];     // initial universe
    Game game;
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
        while ((0 == result) && (! END_OF_FILE))
        {
            END_OF_FILE = !(fgets (line, MAX_LINE, input));
            if (!END_OF_FILE)
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
                        while ( (i < size_x) && (line[i]) )
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
                        if (lineNumGame == size_y)  // finished reading universe, start game
                        {
                            init (&game, cells, size_x, size_y);
                            play (&game);
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
