#include <stdio.h>
#include "MacUILib.h"

// PPA2 GOAL: 
//       Construct the game backbone where the player can control an object 
//       to move freely in the game board area with border wraparound behaviour.

// Watch Briefing Video and Read Lab Manual before starting on the activity!



// PREPROCESSOR CONSTANTS DEFINITION HERE
/////////////////////////////////////////



// GLOBAL VARIABLE DEFINITION HERE
/////////////////////////////////////////

int exitFlag; // Program Exiting Flag - old stuff

int columns = 20; // number of columns on board
int rows = 10; // number of rows on board


// For storing the user input - from PPA1
char input;

// [TODO] : Define objPos structure here as described in the lab document
struct objPos 
{
    int x;// x-coordinate of an object
    int y;// y-coordinate of an object
    char symbol;// The ASCII symbol of the object to be drawn on the screen
};
struct objPos player;

// [TODO] : Define the Direction enumeration here as described in the lab document
//          This will be the key ingredient to construct a simple Finite State Machine
//          For our console game backbone.
enum FSMMode {LEFT, RIGHT, UP, DOWN}; // defines directions of player
enum FSMMode myFSMMode;


// FUNCTION PROTOTYPING DEFINITION HERE
/////////////////////////////////////////

void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);

// You may insert additional helper function prototypes below.
// 
// As a good practice, always insert prototype before main() and implementation after main()
// For ease of code management.



// MAIN PROGRAM LOOP
/////////////////////////////////////////
// This part should be intuitive by now.
// DO NOT TOUCH

int main(void)
{
    Initialize();

    while(!exitFlag)  
    {
        GetInput();

        RunLogic();

        DrawScreen();

        LoopDelay();
    }

    CleanUp();

}


// INITIALIZATION ROUTINE
/////////////////////////////////////////
void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    input = 0; // NULL
    exitFlag = 0;  // not exiting    

    // [TODO] : Initialize more variables here as seen needed.
    //          PARTICULARLY for the structs!!

    // player starting position
    player.x = (columns - 1) / 2;
    player.y = (rows - 1) / 2;
    player.symbol = '@';
    
    myFSMMode = LEFT; // No other FSM initialization required

}


// INPUT PROCESSING ROUTINE
/////////////////////////////////////////
void GetInput(void)
{
    // [TODO] : Implement Asynchronous Input - non blocking character read-in    
    //          (Same as PPA1)
    
    // check if character pressed
    input = MacUILib_hasChar();

    if(input != 0)  // if not null character
    {
        // checks if only ANSCII character pressed
        if (input >= 0 && input <= 127)
        {
            // stores character input only
            input = MacUILib_getChar();
        } 
    }
}

// PROGRAM LOGIC ROUTINE
/////////////////////////////////////////
void RunLogic(void)
{
    // [TODO] : First, process the input by mapping
    //          WASD to the corresponding change in player object movement direction

    // [TODO] : Next, you need to update the player location by 1 unit 
    //          in the direction stored in the program
    
    if(input != 0)  // if not null character
    {
        switch(myFSMMode) 
        {
            // if player going left
            case LEFT: 
                if(input == 'a') // My TOGGLE input trigger state transition
                {
                    player.x--;
                }
                // moving left --> up and down command allowed
                else if (input == 'w' || input == 's')
                {
                    // Transition to UP or DOWN state
                    if (input == 'w')
                    {
                        myFSMMode = UP; // next iteration player moves up
                    }
                    else
                    {
                        myFSMMode = DOWN; // next iteration player moves down
                    }
                }
                break;

            case RIGHT:
                if(input == 'd') // My TOGGLE input trigger state transition
                {
                    player.x++;
                }
                // moving right = up and down command allowed
                else if (input == 'w' || input == 's') 
                {
                    // Transition to UP or DOWN state
                    if (input == 'w')
                    {
                        myFSMMode = UP;
                    }
                    else
                    {
                        myFSMMode = DOWN;
                    }
                }    
                break;

            case UP:
                if(input == 'w')
                {
                    player.y--;
                }
                // moving up = left and right command allowed
                else if (input == 'a' || input == 'd') 
                {
                    // Transition to LEFT or RIGHT state
                    if (input == 'a') 
                    {
                        myFSMMode = LEFT;
                    } 
                    else 
                    {
                        myFSMMode = RIGHT;
                    }
                }
                break;
            
            case DOWN:
                if (input == 's')
                {
                    player.y++;
                }
                // moving down = left and right command allowed
                else if (input == 'a' || input == 'd') 
                {
                    // Transition to LEFT or RIGHT state
                    if (input == 'a') 
                    {
                        myFSMMode = LEFT;
                    } 
                    else 
                    {
                        myFSMMode = RIGHT;
                    }
                }
                break;

            default:
                break;
        
        input = 0;

        }

    if (input == 27)
        {
            // exit on ESC
            exitFlag = 1;
        }

    }

    // [TODO] : Heed the border wraparound!!!
    
    // x-coordinate of player wraparound
    if (player.x == 0) 
    {
        player.x = columns - 2; // Wrap to the right edge
    } 
    else if (player.x == columns - 1) 
    {
        player.x = 1; // Wrap to the left edge
    }

    // y-coordinate of player wrap around
    if (player.y == 0) 
    {
        player.y = rows - 2; // Wrap to the right edge
    } 
    else if (player.y == rows - 1) 
    {
        player.y = 1; // Wrap to the left edge
    }

}


// SCREEN DRAWING ROUTINE
/////////////////////////////////////////
void DrawScreen(void)
{
    // [TODO] : Implement the latest drawing logic as described in the lab manual
    //
    //  1. clear the current screen contents
    MacUILib_clearScreen();

    //  2. Iterate through each character location on the game board
    //     using the nested for-loop row-scanning setup.

    int r = 0;
    int c = 0;
    char boundary = '#'; // Define boundary as a character, not a string

    // goes through each row
    for (r = 0; r < rows; r++) 
    {
        // goes through each column
        for (c = 0; c < columns; c++) 
        {
            // prints '#' only at first and last row and column
            if (r == 0 || r == (rows - 1) || c == 0 || c == (columns - 1)) 
            {
                // print boundary
                MacUILib_printf("%c", boundary); // print a character
            } 

            // print player at its designated x and y
            else if (r == player.y && c == player.x) 
            {
                MacUILib_printf("%c", player.symbol);
            } 

            else 
            {
                // print empty spaces
                MacUILib_printf(" ");
            }
        }
        // moves to the next line
        MacUILib_printf("\n");
    }

    //  3. For every visited character location on the game board
    //          If on border on the game board, print a special character
    //          If at the player object position, print the player symbol
    //          Otherwise, print the space character
    //     Think about how you can format the screen contents to achieve the
    //     same layout as presented in the lab manual

    //  4. Print any debugging messages as seen needed below the game board.
    //     As discussed in class, leave these debugging messages in the program
    //     throughout your dev process, and only remove them when you are ready to release
    //     your code. 

}



// PROGRAM LOOOP DELAYER ROUTINE
/////////////////////////////////////////
void LoopDelay(void)
{
    // Change the delaying constant to vary the movement speed.
    MacUILib_Delay(100000);    
}



// PROGRAM CLEANUP ROUTINE
/////////////////////////////////////////
// Recall from PPA1 - this is run only once at the end of the program
// for garbage collection and exit messages.
void CleanUp(void)
{
    MacUILib_uninit();
}

