#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 60
// we need to store the initial state, which means we need 251 slots in our history if we want to generate 250 generations
#define MAX_GENERATIONS 251
#ifdef NDEBUG
  #define PRINT_GENERATIONS 10
#else
  #define PRINT_GENERATIONS MAX_GENERATIONS
#endif

// we use -1 to indicate no cycle since we use the generation number (which includes 0) to say that we have a cycle start point
const int noCycle = -1;

typedef enum {false, true} boolean;

// an object that simply holds the 2D array. *much* easier to create an array of this struct
struct UNIVERSE
{
  // we use false to indicate dead and true to indicate alive
  boolean state[MAX_DIMENSION][MAX_DIMENSION];
};

typedef struct UNIVERSE Universe;

// an object to track all the information relevant to a run of our game
struct GAME_STATE
{
  int rows;
  int cols;
  int currGen;
  Universe history[MAX_GENERATIONS];
};

typedef struct GAME_STATE GameState;

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

GameState life;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

void checkState();
void clearUniverse(const int generation);
void printUniverse(const int generation);
void loadUniverse();
int cycleCheck();
void nextGeneration();
void playGame();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------


int main()
{
  #define MAX_LINE 80
  char line[MAX_LINE];
	
  while (fgets(line, MAX_LINE, stdin))
  {
    // show the game title, nice that there are no errors in the file...
    printf("%s", line);

    loadUniverse();
    // show the state prior to running the game
    printUniverse(0);
    playGame();
    
    printf("\n");
  }
 
  return EXIT_SUCCESS;
}

void checkState()
{
  assert(life.rows > 0);
  assert(life.rows <= MAX_DIMENSION);
  assert(life.cols > 0);
  assert(life.cols <= MAX_DIMENSION);
  assert(life.currGen >= 0);
  assert(life.currGen < MAX_GENERATIONS);
}

// make sure everything we may have made alive is now dead, relative to our current universe
void clearUniverse(const int generation)
{
  int row, col;
  
  assert(generation >= 0);
  assert(generation < MAX_GENERATIONS);
  checkState();
  
  if (generation >= 0 && generation < MAX_GENERATIONS &&
      life.rows > 0 && life.rows <= MAX_DIMENSION &&
      life.cols > 0 && life.cols <= MAX_DIMENSION)
  {
    for (row = 0; row < life.rows; row++)
    {
      for (col = 0; col < life.cols; col++)
      {
        life.history[generation].state[row][col] = false;
      }
    }
  }
  
  checkState();
}

void printUniverse(const int generation)
{
  int row, col;
  
  assert(generation >= 0);
  assert(generation < MAX_GENERATIONS);
  checkState();

  if (generation >= 0 && generation < MAX_GENERATIONS &&
      life.rows > 0 && life.rows <= MAX_DIMENSION &&
      life.cols > 0 && life.cols <= MAX_DIMENSION)
  {
    printf("Generation %d:\n", generation);
    
    // we use -1/+1 so we can draw the borders
    for (row = -1; row < life.rows+1; row++)
    {
      for (col = -1; col < life.cols+1; col++)
      {
        if (-1 == row || life.rows == row)
        {
          if (-1 == col || life.cols == col)
          {
            printf("+");
          }
          else
          {
            printf("-");
          }
        }
        else if (-1 == col || life.cols == col)
        {
          printf("|");
        }
        else
        {
          printf("%c", (life.history[generation].state[row][col] ? '*' : '.'));
        }
      }
      printf("\n");
    }
  }
	
  checkState();
}

void loadUniverse()
{
  int row, col;
  int count;
  char ch;
  
  // we always (re-)start a universe at generation 0
  life.currGen = 0;
  
  count = scanf("%d %d", &(life.rows), &(life.cols));
  assert(2 == count);
  checkState();
  
  if (2 == count &&
      life.rows > 0 && life.rows <= MAX_DIMENSION &&
      life.cols > 0 && life.cols <= MAX_DIMENSION)
  {
    // clear the newline...
    getchar();
    
    // start with a clean (all dead) state, then we only need to worry about turning on living cells
    clearUniverse(0);
    
    for (row = 0; row < life.rows; row++)
    {
      for (col = 0; col < life.cols; col++)
      {
        ch = getchar();
        
        if ('X' == ch)
        {
          life.history[0].state[row][col] = true;
        }
      }
      
      // clear out the end of line character
      ch = getchar();
    }
  }

  checkState();
}

void playGame()
{
  int generation;
  int cycle = noCycle; // start with the default value for cycleCheck()...
  
  checkState();
	
  if (life.currGen >= 0 && life.currGen < MAX_GENERATIONS)
  {
    // note that we need to check that we're within range for the *next* generation
    while (life.currGen+1 < MAX_GENERATIONS && noCycle == cycle)
    {
      nextGeneration();
      cycle = cycleCheck();
    }
    
    if (cycle != noCycle)
    {
      printf("Found a cycle between generation %d and generation %d\n", cycle, life.currGen);
    }
    
    // default to start printing at 1 since that's our first "new" generation
    generation = 1;
    
    // determine if we need to limit the number of generations we print
    if (life.currGen > PRINT_GENERATIONS)
    {
      generation = life.currGen - PRINT_GENERATIONS + 1;
    }
    
    while (generation <= life.currGen)
    {
      printUniverse(generation);
      generation++;
    }
  }
  
  checkState();
}

// returns the generation where a cycle was detected, noCycle if none found
int cycleCheck()
{
  boolean cycleFound = false;
  int cycle = noCycle;
  
  checkState();
	
  if (life.currGen >= 0 && life.currGen < MAX_GENERATIONS &&
      life.rows > 0 && life.rows <= MAX_DIMENSION &&
      life.cols > 0 && life.cols <= MAX_DIMENSION)
  {
    for (int next=0; next<life.currGen && !cycleFound; next++)
    {
      // start by assuming we're going to find a cycle -- then a single difference will stop our check for this generation
      cycleFound = true;
      for (int row=0; row<life.rows && cycleFound; row++)
      {
        for (int col=0; col<life.cols && cycleFound; col++)
        {
          if (life.history[life.currGen].state[row][col] != life.history[next].state[row][col])
            cycleFound = false;
        }
      }
      
      if (cycleFound)
        cycle = next;
    }
  }
  
  assert(cycle == noCycle || (cycle >= 0 && cycle < life.currGen));
  checkState();
	
  return cycle;
}

void nextGeneration()
{
  int row, col;
  int nextGen = life.currGen+1;
	
  assert(nextGen >= 0);
  assert(nextGen < MAX_GENERATIONS);
  checkState();
  
  if (life.currGen >= 0 && life.currGen < MAX_GENERATIONS &&
      nextGen >= 0 && nextGen < MAX_GENERATIONS &&
      life.rows > 0 && life.rows <= MAX_DIMENSION &&
      life.cols > 0 && life.cols <= MAX_DIMENSION)
  {
    // start with everything dead...
    clearUniverse(nextGen);
    
    for (row = 0; row < life.rows; row++)
    {
      for (col = 0; col < life.cols; col++)
      {
        int neighbours = 0;
        
        // to calculate the number of neighbours I'm "cheating": true == 1 so I can just add up the true flags
        // we also assume that off the edge of the universe is always dead
        
        // for all cases, always make sure we don't go off the sides of the universe...
        // since our universe is in [0,rows/cols-1], we can't go beyond that when we do +/- 1
        
        if (row > 0)
        {
          neighbours += life.history[life.currGen].state[row-1][col];
          
          if (col > 0)
            neighbours += life.history[life.currGen].state[row-1][col-1];
          if (col < life.cols-1)
            neighbours += life.history[life.currGen].state[row-1][col+1];
        }
        
        if (col > 0)
          neighbours += life.history[life.currGen].state[row][col-1];
        if (col < life.cols-1)
          neighbours += life.history[life.currGen].state[row][col+1];
        
        if (row < life.rows-1)
        {
          neighbours += life.history[life.currGen].state[row+1][col];
          
          if (col > 0)
            neighbours += life.history[life.currGen].state[row+1][col-1];
          if (col < life.cols-1)
            neighbours += life.history[life.currGen].state[row+1][col+1];
        }
        
        // now we can determine the new state of this cell
        // note that by setting everything to dead at the beginning, we only need to set cells that are alive
        
        if (life.history[life.currGen].state[row][col])
        {
          // current cell is alive
          if (neighbours == 2 || neighbours == 3)
            life.history[nextGen].state[row][col] = true;
        }
        
        else
        {
          // current cell is dead
          if (neighbours == 3)
            life.history[nextGen].state[row][col] = true;
        }
      }
    }
    
    life.currGen = nextGen;
	}
  
  checkState();
}

