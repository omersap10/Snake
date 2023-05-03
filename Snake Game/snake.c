/*ID - 206992505
Description of the prorgam - the prorgam is the snake game.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key

#define ROWS 26 //num of rows in board+1 
#define COLS 76  //num of cols in board+1 

//new struct called snake with two int variables in order to know where the snake links are on the board.
struct snake
{
	int x;
	int y;
};
typedef struct snake snake;

//funcations declerion:
int EnterMenu(); 
void playGame(); 
void printGameLines(char board[][COLS]);
void gameMove(char board[][COLS], snake Snake[]);
void foodPlacement(char board[][COLS]);
void gotoxy(int y, int x);
int getKey();
bool checkMove(char board[][COLS], snake Snake[], int x, int y);
void moveSnake(char board[][COLS], snake Snake[], int x, int y, int* minSize, int* maxSize);
bool isVaildInput(int validInput);

void main()
{
	int choice = EnterMenu;

	while (choice != 1 && choice != 0) //if chose 2,3,4 ask for another choise
	{
		choice = EnterMenu();
	}
	
	if (choice == 1) //for basic level
	{
		playGame();
		main();
	}

	else //if chose exit
	{
		printf("Goodbye!\n");
			return;
	}
}

//functions:

/*EnterMenu:
The purpose: The function will show to main menu and ask for a choice. return the choice of game type by the player.

Parameters:
choice - local variable for the player choice.  
*/

int EnterMenu()
{
	int choice;

	printf("*************************************************\n");
	printf("*Welcome to the SNAKE, Pick your level:\n* 0 - Exit\n* 1 - Basic\n* 2 - Intermidiate\n* 3 - Advanced\n");
	printf("*************************************************\n");

	scanf("%d", &choice);

	return choice;
}

/*playGame:
The purpose: The function will define two arrays (snake and board) and start the game by calling print board function and game move function which will start the game.

Parameters:
board - 2D array for the game board.
Snake - the snake on the board, top size of 20 links.
*/

void playGame() //the game itself
{
	char board[ROWS][COLS];
	snake Snake[20];

	system("CLS"); //clean the screen
	printGameLines(board); //print board
	gameMove(board, Snake); //move snake
	return;
}

/*printGameLines:
The purpose: The function will print the board lines.

Parameters:
board - 2D array for the game board.
ROWS - the number of rows of the board.
COLS - the number of cols of the board.
rows - local variable for loops and checks on the board.
cols - local variable for loops and checks on the board.
*/

void printGameLines(char board[][COLS]) //print border lines
{
	int row, col; //local variable for loop

	for (row = 0; row < ROWS; row++)
	{
		for (col = 0; col < COLS; col++)
		{
			if (row == ROWS - 1 || col == COLS - 1 || row == 0 || col == 0)
			{
				printf("#");
				board[row][col] = '#';
			}
			
			else
			{
				printf(" ");
				board[row][col] = ' ';
			}
		}
		
		printf("\n");
	}
}

/*gameMove:
The purpose: The function will get from the palyer the next move of the snake and check for lose or win in the game.

Parameters:
board - 2D array for the game board.
Snake - the snake on the board, top size of 20 links.
maxSize - the starting size of the snake by the game definitions.
minSize - the actual size of the snake from the beginning of the game.
x,y - coordinate of cells on board.
c - the direction of the snake, changing by the player choice.
validInput - variable for the check if the player move is valid.
*/

void gameMove(char board[][COLS], snake Snake[])
{
	int maxSize = 10; //the start size of the snake for the player
	int minSize = 0; //the real size of the snake by the start of the game
	int validInput;
	char c;
	int x = 40, y = 12;  // x for coulmn and y for line 

	foodPlacement(board); //place first food on board

	// start from the middle of the screen
	gotoxy(x, y);
	// the direction is Right untill an arrow hit
	c = RIGHT;

	while (c != ESC)
	{
		// sleeps for a second before continuing to the next command
		Sleep(1000);

		// only if a key was hit we read what key code it was
		if (_kbhit()) // if any key was hit
		{
			validInput = getKey();
			if(isVaildInput(validInput) == true)
			c = validInput;  // change direction
		}
		switch (c)
		{
		case RIGHT:
			x++;
			break;
		case DOWN:
			y++;
			break;
		case LEFT:
			x--;
			break;
		case UP:
			y--;
			break;
		}

		if (c == ESC) //if press escape go back to main menu
		{
			system("CLS");
			return;
		}

		if (maxSize == 20) //if win - show winning declerion and back to main menu
		{
			system("CLS");
			printf("You Win!\n\n");
			Sleep(1000);
			system("CLS");
			return;
		}

		if ((checkMove(board, Snake, x, y)) == false) //if lose - show lose declerion and back to main menu
		{
			system("CLS");
			printf("Game Over!\n\n");
			Sleep(1000);
			system("CLS");
			return;
		}

		moveSnake(board, Snake, x, y, &minSize, &maxSize); //move the snake links
		gotoxy(x, y); //next step of snake
		printf("@");
		gotoxy(26, 81);
	}
}

/*foodPlacement:
The purpose: The function will place a snake food ($) on the board.

Parameters:
board - 2D array for the game board.
FoodX - coordinate of cells on board.
FoodY - coordinate of cells on board.
*/

void foodPlacement(char board[][COLS])
{
	int FoodX, FoodY;

	srand(time(NULL));

	FoodX = rand() % 23; //output value of random number from 0 to 23.
	FoodY = rand() % 73; //output value of random number from 0 to 73.

	board[FoodX][FoodY] = '$';
	gotoxy(FoodY, FoodX);
	printf("$");
} 

/*checkMove:
The purpose: The function will check for a fail in game by the player -  by touching a fence or the snake touch himselft.

Parameters:
board - 2D array for the game board.
Snake - the snake on the board, top size of 20 links.
x,y - coordinate of cells on board.
*/

bool checkMove(char board[][COLS], snake Snake[], int x, int y) 
{
	if (board[y][x] == '@' || board[y][x] == '#') //if touching a fence or himselft
		return false;
	else
		return true;
}

/*moveSnake:
The purpose: The function will move the snake on the board - if the snake size (on the start of the game) will be less then 10 the snake will grow up untill 10.
after reaching 10 for each move the snake tail will delete and the head promote forward. if the snake will eat food one link Will be added to the snake.

Parameters:
board - 2D array for the game board.
Snake - the snake on the board, top size of 20 links.
maxSize - the starting size of the snake by the game definitions.
minSize - the actual size of the snake from the beginning of the game.
x,y - coordinate of cells on board.
i - local variable for loop.
*/

void moveSnake(char board[][COLS], snake Snake[], int x, int y, int* minSize, int* maxSize)
{
	int i;

	if (board[y][x] == '$') //if eat make the snake one size larger
	{
		(*maxSize)++;
		foodPlacement(board);
	}
	
	if (*minSize < *maxSize) //if the snake is smaller then 10 (the starting size)
		(*minSize)++;

	else //one move of the snake (if bigger then 10), delete the tail and promote the head of the snake
	{
		board[Snake[*minSize - 1].y][Snake[*minSize - 1].x] = ' ';
		gotoxy(Snake[*minSize - 1].x, Snake[*minSize - 1].y);
		printf("%c", board[Snake[*minSize - 1].y][Snake[*minSize - 1].x]);
	}

	board[y][x] = '@'; //change the next cell to snake link

	for (i = (*minSize - 1); i > 0; i--) //promote the snake
		Snake[i] = Snake[i - 1];

	Snake[0].x = x;
	Snake[0].y = y;	
}

void gotoxy(int y, int x)
{
	printf("\x1b[%d;%df", x + 1, y + 1);
}

int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

/*isVaildInput:
The purpose: Boolean function that check that the player moves are vaild, if not that snake will keep moving on the same direction.

Parameters:
validInput - variable for the check if the player move is valid and contain the player key choice.
*/

bool isVaildInput(int validInput)
{
	if (validInput == RIGHT || validInput == LEFT || validInput == UP || validInput == DOWN || validInput == ESC)
		return true;
	else
		return false;
}