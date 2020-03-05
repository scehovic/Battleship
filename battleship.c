/*----------------------------------------------------------------------------
	Includes
-----------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
/*----------------------------------------------------------------------------
			       Defines
-----------------------------------------------------------------------------*/
typedef struct ships_struct{
	char* name; //name of boat
	int length; //size of boat
	char column; //column for boat
	int row; //row for boat
	int orientation; //whether the boat is horizontal or vertical.
}ships;

/*----------------------------------------------------------------------------
                      	    Prototypes
-----------------------------------------------------------------------------*/
int isGameOver(int pHits, int cHits);
int isHit(char ocean[][6], int row, int col);
void generateGame(char ocean[][6]);
void scanUserShips(ships array[], char  playerOcean[][6]);
void scanCpuShips(ships array[],  char  cpuOcean[][6]);


/*----------------------------------------------------------------------------
                              Notes
-----------------------------------------------------------------------------*/
/* 
Compile with gcc battleship.c -o battleship -lncurses
 Run with ./battleship
 We don’t need the DS4 controller
MAKE SURE TO INCLUDE COMMENTS WHEN YOU ADD CODE */

/*----------------------------------------------------------------------------
		Implementation
-----------------------------------------------------------------------------*/

void main()
{

	srand(time(NULL)); //seed random
	
	//give all variables a value 
	char guessCol = 'A';
	int	guessRow = 0;
	int cpuGuessCol = 0;
	int cpuGuessRow = 0;
	int playerHits = 0; 
	int cpuHits = 0;
	

	char playerOcean[6][6]; //board of player ships
	
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			playerOcean[i][j] = '?';
		}
	}
	
	char cpuOcean[6][6]; // board of cpu ships
	
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			cpuOcean[i][j] = '?';
		}
	}
	
	
	char guessOcean[6][6]; // board of guesses
	
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			guessOcean[i][j] = '?';
		}
	}

//the 6 spots the user’s ships are in, 3 boats total, 3x1, 2x1, 1x1
//declares the array of ships for the player, and calls the function to have the user initialize.
	ships playerShips[3];
	playerShips[0].name = "Carrier";
	playerShips[0].length = 3;
	playerShips[1].name = "Destroyer";
	playerShips[1].length = 2;
	playerShips[2].name = "Patrol";
	playerShips[2].length = 1;

	scanUserShips(playerShips, playerOcean);

//declares the array of ship for the computer, and calls a function to initialize
	ships cpuShips[3];
	cpuShips[0].name = "Carrier";
	cpuShips[0].length = 3;
	cpuShips[1].name = "Destroyer";
	cpuShips[1].length = 2;
	cpuShips[2].name = "Patrol";
	cpuShips[2].length = 1;

	scanCpuShips(cpuShips, cpuOcean);

	initscr(); // opens ncurses
	refresh(); 

	generateGame(playerOcean); //generate the game
	
	mvprintw(0, 0, "To win you need to get 6 hits!");
	mvprintw(2, 8, "Hits on computer: %d", playerHits);
	mvprintw(12, 8, "Hits on user: %d", cpuHits);
	
	while(isGameOver(playerHits, cpuHits) == 0) // check if game over
	{

		mvprintw(18, 1, "Please enter your guess: ");
		mvscanw(19, 1, " %c%d", &guessCol, &guessRow);
		if((guessCol < 71 && guessCol > 64) && (guessRow < 7 && guessRow > 0) && (guessOcean[guessRow-1][guessCol-65] == '?')){ // checks if in range 
			
			if(isHit(cpuOcean, guessRow, (guessCol - 65)) == 1){
				guessOcean[guessRow-1][guessCol - 65] = 'X';
				playerHits++;
			}
			else{
				guessOcean[guessRow-1][guessCol - 65] = 'M';
			}
		}
		else{
			mvprintw(19, 1, "Sorry invalid guess, out of bounds, or on top of a previous guess. You lose your turn.");
		}
		
		refresh();
		sleep(2); // delay to build anticipation of if your guess was hit or miss
		
		while(1){
			//random number 0-5 for the computers guess
			cpuGuessRow = rand() % 6; 
			cpuGuessCol = rand() % 6;
			
			//Ensures guess was not already guessed by the computer
			if(playerOcean[cpuGuessRow][cpuGuessCol] == 'O' || playerOcean[cpuGuessRow][cpuGuessCol] == '?'){
				if(isHit(playerOcean, cpuGuessRow, cpuGuessCol) == 1){
					playerOcean[cpuGuessRow][cpuGuessCol] = 'X';
					cpuHits++;
					break;
				}
				else{
					playerOcean[cpuGuessRow][cpuGuessCol] = 'M';
					break;
				}
			}
		}

		//shows your guess placement
		for(int k = 1; k <= 6; k++)
		{

			for(int j = 1; j <= 6; j++)
			{
				mvprintw(k, j, "%c", guessOcean[k-1][j-1]);
			}

		}
		
		mvprintw(2, 8, "Hits on computer: %d", playerHits);

		//shows your boat placement
		for(int k = 11; k <= 16; k++)
		{

			for(int j = 1; j <= 6; j++)
			{
				mvprintw(k, j, "%c", playerOcean[k-11][j-1]);
			}

		}
		
		mvprintw(12, 8, "Hits on user: %d", cpuHits);
				
		//clear off the old guess
		mvprintw(19, 1, "                                                                                                   ");
		mvprintw(20, 1, "                                                                     ");
		
		refresh();
	}
	
	//Close window
	endwin();
	
	//declare who won
	if(playerHits == 6){
			printf("\nYou Won! Congrats dude!");
	}
	if(cpuHits == 6){
			printf("\nYou Lost! Nice try though! Better luck next time!");
	}
	
	
	
}

int isGameOver(int pHits, int cHits)
{
	if((pHits == 6) || (cHits == 6)){ //check if either player has sunk all the ships.
		return 1;
	}
	else{
		return 0;
	}
}

//function to check if the bomb hit a boat
int isHit(char  ocean[][6], int row, int col){
		if(ocean[row][col] == 'O'){
			return 1;
		} else {
			return 0;
		}
}

void generateGame(char ocean[][6]){
// Sets up board that will show your guesses
	mvprintw(0, 1, "Guess Board");
	for(int k = 1; k <= 6; k++)
	{

		for(int j = 1; j <= 6; j++)
		{
			mvprintw(k, j, "%c", '?');
		}

	}

	//Sets up board showing your boat placement
	mvprintw(10, 1, "Your board");
	for(int k = 11; k <= 16; k++)
	{

		for(int j = 1; j <= 6; j++)
		{
			mvprintw(k, j, "%c", ocean[k-11][j-1]);
		}

	}
	refresh();
}

void scanUserShips(ships array[], char playerOcean[][6]){
	bool exit = false;
	while(1){
		for(int i = 0; i < 3; i++){
			printf("Enter column (A-F) for the first segment of your %s ship:\n", array[i].name);
			scanf(" %c", &array[i].column); //gets column for ships

			printf("Enter the row (1-6) for the first segment of your %s ship:\n", array[i].name);
			scanf("%d", &array[i].row); //gets rows for ships

			printf("Enter the orientation (0 for horizontal, 1 for vertical) for you %s ship:\n", array[i].name);
			scanf("%d", &array[i].orientation); //gets orientation for ships

			for(int k = 0; k < array[i].length; k++){ //store where the ship is located
				if(array[i].orientation == 0){
					if(playerOcean[array[i].row - 1][array[i].column - 65 + k] == '?'){
						playerOcean[array[i].row - 1][array[i].column - 65 + k] = 'O';
					}
					else{
						array[i].row = 12; //make it so the loop stops and it says ivalid coordinates
						k = 3;
					}
				}
				if(array[i].orientation == 1){
				 if(playerOcean[array[i].row - 1 + k][array[i].column - 65] == '?'){
						playerOcean[array[i].row - 1 + k][array[i].column - 65] = 'O';
					}
					else{
						array[i].row = 12;
							k = 3;
						}
				}

			}
			// check if in range
			if((array[i].column < 71 && array[i].column > 64) && (array[i].row < 7 && array[i].row > 0) && (array[i].orientation == 1 || array[i].orientation == 0)){
				if(i == 2){
					exit = true;
				}
				//checks if the ship info is able to be used, if not, recalls the function
			}
			else{
				printf("Invalid coordinates. Some ships out of bounds or on top of other ships.\nPlease try again.\n");
				i = -1;
				for(int p = 0; p < 6; p++){
					for(int s = 0; s < 6; s++){
					playerOcean[p][s] = '?';
					}
				}	
			}
		}
		
		if(exit){
			break;
		}
	}
}

void scanCpuShips(ships array[], char cpuOcean[][6]){
	bool exit = false;
	while(1){
		for(int i = 0; i < 3; i++){
			array[i].column = rand() % 6 + 65; //randomly assigns column
			array[i].row = rand() % 6 + 1; //randomly assigns row
			array[i].orientation = rand() % 2; //Randomly assigns an orientation

			for(int k = 0; k < array[i].length; k++){ //store where the ship is located
				if(array[i].orientation == 0){
					if(cpuOcean[array[i].row + k][array[i].column - 65] == '?'){
						cpuOcean[array[i].row + k][array[i].column - 65] = 'O';
					}
					else{
						array[i].row = 12; //make it so the loop stops and it says ivalid coordinates
						k = 3;
					}
				}
				if(array[i].orientation == 1){
					if(cpuOcean[array[i].row][(array[i].column - 65) + k] == '?'){
						cpuOcean[array[i].row][(array[i].column - 65) + k] = 'O';
					}
					else{
						array[i].row = 12;
						k = 3;
					}
				}
			}
			if((array[i].column < 71 && array[i].column > 64) && (array[i].row < 7 && array[i].row > 0) && (array[i].orientation == 1 || array[i].orientation == 0)){
			//checks if the ship info is able to be used, if not, recalls the function
				if(i == 2){
					exit = true;
				}
			}
			else{
				i = -1;
				for(int p = 0; p < 6; p++){
					for(int s = 0; s < 6; s++){
						cpuOcean[p][s] = '?';
					}
				}
			}
		}
		if(exit){
			break;
		}
	}
}



