#define _CRT_SECURE_NO_WARNINGS		//This is to stop errors due to scanf()
#include <stdio.h>		//This is for the printf() and getchar() function
#include <time.h>		//This is for the srand function seed
#include <ctype.h>		//This is for the toupper function

#define TRUE 1		//This allows TRUE to substitute for 1
#define FALSE 0		//This allows FALSE to substitute for 0
#define player1 0		//player1 is used when managing turns to represent player1
#define player2 1		//player2 is used when managing turns to represent player2
#define emptySpot 0		//emptySpot is the value of an empty spot on the board
#define player1Pawn 1		//player1Pawn is the value of an player1s pawn on the board
#define player1King 2		//player1King is the value of an player1s king on the board
#define player2Pawn -1		//player2Pawn is the value of an player2s pawn on the board
#define player2King -2		//player2King is the value of an player2s king on the board
#define boardSize 8		//This is the size of the board, both the X and the Y dimensions
#define chipSelect 0		//This is a named stage for the main loop, This stage manages the user selecting their chip
#define targetSelect 1		//This is a named stage for the main loop, This stage manages the user selecting their target
#define moveChip 2		//This is a named stage for the main loop, This Stage calls moveAttempt(), then acts according to its output
#define turnFinish 3		//This is a named stage for the main loop, This Stage will check for new kings, sees if the game is won, and gives the next turn to the other player
#define moveNormal 0		//This represents a valid non-aggressive move from the moveAttempt()
#define moveAggressive 1		//This represents a valid aggressive move from the moveAttempt()
#define moveInvalid 2		//This represents a invalid move from the moveAttempt()

void welcome();		//This will say hello and the display the rules
void boardPrint(int board[boardSize][boardSize]);		//This will clear the screen, then print the current board
int selector(int *rowR, int *columnR);		//This will allow the user to enter coordinates on the board
int moveAttempt(int board[boardSize][boardSize], int rowCurrent, int columnCurrent, int rowTarget, int columnTarget, int aggressive);		//This checks if a move is valid and if so enacts the move
void chipCounter(int board[boardSize][boardSize], int *player1ChipC, int *player2ChipC);		//This counts the chips for both players

void main(void)		//This will contain the main loop, as well as initiate the shared variables
{
	const int Player2AI = FALSE;		//This will be true if game is PVE and false if PVP
	int currentPlayer = player1;		//This will hold 0 for the first player, or 1 for the second player
	int gameDone = FALSE;		//Main loop will continue as long as this is false
	int turnStage = 0;		//This will hold the current stage of any given turn
	int rowCurrent;		//This is the row of the selected chip
	int columnCurrent;		//This is the column of the selected chip
	int rowTarget;		//This is the row of the target spot
	int columnTarget;		//This is the row of the target spot
	char answer;		//This is to hold the users answer for Y/N questions
	int moveState;		//This hold the output of moveAttempt()
	int aggressive = FALSE;		//This allows the user to return from stage moveChip as long as its 0
	int aggressiveLoopDone = FALSE;//The selector loop will continue as long as this is false
	int player1ChipC;		//This is the number of player1s chips left
	int player2ChipC;		//This is the number of player2s chips left

							//This creates the board, 0 is an emptySpot, 1 for a player1Pawn, and -1 for a player2Pawn, !THIS WILL NEED TO BE MANUALLY UPDATED IF boardSize IS CHANGED!
	int board[boardSize][boardSize] = { { 0,-1, 0,-1, 0,-1, 0,-1 },
	{ -1, 0,-1, 0,-1, 0,-1, 0 },
	{ 0,-1, 0,-1, 0,-1, 0,-1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 1, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0 } };

	srand(time(NULL));		//This Primes the random number Generator
	currentPlayer = rand() % 2;		//This randomly finds who moves first, Although normally its the controller of the darker chips that moves first or in this case currentPlayer = 0
	welcome();		//A simple hello
	boardPrint(board);		//This will print the board for the first time
	chipCounter(board, &player1ChipC, &player2ChipC);		//This counts and displays the chips for both players

	printf("Player%ds turn\n", currentPlayer + 1);		//This states whos turn it is at the start of the game
	while (!gameDone)		//This is the main loop
	{
		if (currentPlayer == player1 || !Player2AI)		//This is for player1/Humans
		{



			if (turnStage == chipSelect)		//This stage manages the user selecting their chip
			{
				printf("Select your chip you want to move\n");
				if (selector(&rowCurrent, &columnCurrent) == 0)		//This calls selector() and if it returns 0 it then verifies it
				{
					if (currentPlayer == player1)
					{
						if (board[rowCurrent][columnCurrent] > emptySpot)		//Checks to see if selected spot has a chip that is greater then 0, implying that its player1Pawn/1 or player1King/2
						{
							printf("You selected %d%c\n", rowCurrent + 1, columnCurrent + 65);
							turnStage = targetSelect;		//sets the loop to its next stage
						}
						else		//If the spot does not have a valid chip, it will complan and restart the loop, asking for a new chip with selector()
						{
							printf("You dont have a piece there\n");
						}
					}
					else if (currentPlayer == player2)
					{
						if (board[rowCurrent][columnCurrent] < emptySpot)		//Checks to see if selected spot has a chip that is less then 0, implying that its player2Pawn/-1 or player2King/-2
						{
							printf("You selected %d%c\n", rowCurrent + 1, columnCurrent + 65);
							turnStage = targetSelect;		//sets the loop to its next stage targetSelect
						}
						else		//If the spot does not have a valid chip, it will complan and restart the loop, asking for a new chip with selector()
							printf("You dont have a piece there\n");
					}
					else		//This should never happen, if it does then it will print an error
					{
						printf("\x1b[31m!Error: Invalid currentPlayer, has value %d!\x1b[0m\n", currentPlayer);
						gameDone = TRUE;		//This ends the game due to error
					}
				}
				else		//If selector() returns a 1, then the user entered the command 0Q
				{
					printf("Do you want to quit?");		//This verifies that the user wants to quit
					scanf(" %c", &answer);
					while (getchar() != '\n');		//This wipes the buffer to prevent errors
					if (answer == 'y' || answer == 'Y')		//If the user does not say yes, restarts the loop, asking for a new chip with selector()
						gameDone = TRUE;
				}
			}



			else if (turnStage == targetSelect)		//This stage manages the user selecting their target
			{
				printf("Select where you want to move your chip\n");
				if (selector(&rowTarget, &columnTarget) == 0)		//This calls selector() and if it returns 0 it then verifies it
				{
					if (board[rowTarget][columnTarget] == emptySpot)		//This checks to see if the target spot is empty, if so it will go to the next stage
					{
						turnStage = moveChip;		//sets the loop to its next stage moveChip
						aggressive = FALSE;		//This primes aggressive so that the player has not move yet
					}
					else		//If the use does not enter a empty target spot, it will complan and restart the loop, asking for a new chip with selector()
						printf("The spot you choose is not empty\n");
				}
				else		//If selector() returns a 1, then the user entered the command 0Q
					turnStage = chipSelect;		//This returns the user to the chipSelect stage to choose a new chip
			}



			else if (turnStage == moveChip)		//This Stage calls moveAttempt(), then acts according to its output
			{
				moveState = moveAttempt(board, rowCurrent, columnCurrent, rowTarget, columnTarget, aggressive);		//This checks if a move is valid and if so enacts the move
				if (moveState == moveNormal)		//If the move is valid and non-aggressive, This will print the new board and start the last stage
				{
					boardPrint(board);
					turnStage = turnFinish;		//sets the loop to its next stage turnFinish
				}
				else if (moveState == moveAggressive)		//If the move is valid and aggressive, This will print the new board and start the procces of subsequent moves
				{
					boardPrint(board);
					aggressive = TRUE;		//This should not be changed back untill the next turn
				}
				else if (moveState == moveInvalid)		//If the move is invalid and turn is still non-aggressive, it will go back to targetSelect, if the move is invalid and turn is aggressive, it will allow the user to enter a new target
				{
					if (!aggressive)
						turnStage = targetSelect;		//This returns the user to the targetSelect stage to choose a new target, and possibly a new chip
				}
				else		//This should never happen, if it does then it will print an error
				{
					printf("\x1b[31m!Error: Invalid return from moveAttempt, has value %d!\x1b[0m\n", moveState);
					gameDone = TRUE;		//This ends the game due to error
				}
				if (aggressive == TRUE)		//If the players move was successful and aggressive, this will ask if they want to move again
				{
					rowCurrent = rowTarget;
					columnCurrent = columnTarget;
					printf("Do you want to move again?\n");		//This asks that the player if they want to move again after an aggressive move
					scanf(" %c", &answer);
					while (getchar() != '\n');		//This wipes the buffer to prevent errors
					if (!(answer == 'n' || answer == 'N'))
					{
						aggressiveLoopDone = FALSE;
						while (!aggressiveLoopDone)		//This loops untill the player selects a valid target of gives up turn
						{
							printf("Select where you want to move your chip\n");
							if (selector(&rowTarget, &columnTarget) == 0)		//This calls selector() and if it returns 0 it then verifies it
							{
								if (board[rowTarget][columnTarget] == emptySpot)		//This will varify that the target spot is empty
									aggressiveLoopDone = TRUE;		//This will stop the loop and allow the new target spot to be attempted
								else
									printf("The spot you choose is not empty\n");		//If invalid, it will restart the loop, allowing the use to input a new target spot
							}
							else		//If selector() returns a 1, then the user entered the command 0Q
							{
								boardPrint(board);
								printf("Do you want to move again?\n");		//This asks that the player if they want to move again after an aggressive move
								scanf(" %c", &answer);
								while (getchar() != '\n');		//This wipes the buffer to prevent errors
								if ((answer == 'n' || answer == 'N'))
								{
									aggressiveLoopDone = TRUE;		//This will stop the loop as the player has given up their turn
									turnStage = turnFinish;		//sets the loop to its next stage turnFinish
								}
							}
						}
					}
					else		//If the user does not say yes, then it starts the other players turn
						turnStage = turnFinish;		//sets the loop to its next stage turnFinish
				}
			}



			else if (turnStage == turnFinish)		//This Stage will check for new kings, sees if the game is won, and gives the next turn to the other player
			{
				turnStage = chipSelect;		//This sets the main loop to its first stage for the other player
				if (currentPlayer == player1)		//This checks the last chip to see if it should now be a king, and gives the next turn to the other player
				{
					if (rowTarget == 0 && board[rowTarget][columnTarget] == player1Pawn)		//This looks to see if the last chip moved is now at the top of the board
					{
						board[rowTarget][columnTarget] = player1King;		//This creates a king for player1
						printf("\x1b[33mA King was crowned for player%d at %d%c\x1b[0m\n", currentPlayer + 1, rowTarget + 1, columnTarget + 'A');
					}
					currentPlayer = player2;
				}
				else
				{
					if (rowTarget == boardSize - 1 && board[rowTarget][columnTarget] == player2Pawn)		//This looks to see if the last chip moved is now at the bottom of the board
					{
						board[rowTarget][columnTarget] = player2King;		//This creates a king for player2
						printf("\x1b[33mA King was crowned for player%d at %d%c\x1b[0m\n", currentPlayer + 1, rowTarget + 1, columnTarget + 'A');
					}
					currentPlayer = player1;
				}
				chipCounter(board, &player1ChipC, &player2ChipC);		//This counts the current chips on the board
				if (player1ChipC == 0)		//If player1 has no more chips, player2 wins
				{
					printf("Player2 WINS!\n");
					gameDone = TRUE;		//This ends the main loop, ending the game
				}
				else if (player2ChipC == 0)		//If player2 has no more chips, player1 wins
				{
					printf("Player1 WINS!\n");
					gameDone = TRUE;		//This ends the main loop, ending the game
				}
				else		//If no one wins, then it starts the next turn
				{
					system("pause");		//This is so that the next player can see the last turn
					boardPrint(board);
					chipCounter(board, &player1ChipC, &player2ChipC);		//This is to print chips for the next player
					printf("Player%ds turn\n", currentPlayer + 1);
				}
			}



		}
		else if (currentPlayer == player2 && Player2AI)		//This is for the AI
		{
			printf("Player%d appears to be dead\n", currentPlayer + 1);
			currentPlayer = player1;
			printf("Player%ds turn\n", currentPlayer + 1);
		}
		else		//This should never happen, if it does then it will print an error
		{
			printf("\x1b[31m!Error: Invalid currentPlayer, has value %d!\x1b[0m\n", currentPlayer);
			gameDone = TRUE;		//This ends the game due to error
		}
	}
	system("pause");		//This will allow the user to see the final output
}

void welcome()		//This will say hello and the display the rules
{
	char answer;
	printf("Welcome to checkers\n");
	printf("Do you want to read the rules?\n");		//This asks that the player if they want to move again after an aggressive move
	scanf(" %c", &answer);
	while (getchar() != '\n');		//This wipes the buffer to prevent errors
	if ((answer == 'y' || answer == 'Y'))
	{
		printf("This game follows the standard rules for checkers\n");
		printf("Player 1 is b/B going up, and player 2 is r/R going down\n");
		printf("Yes or no questions can be answer with y/Y/yes/YES for yes, and n/N/no/NO for no\n");
		printf("When selecting a chip or a target spot, enter row then column, examples are 1a/3D/ 7 f\n");
		printf("To go back enter 0Q at any chip/spot selection\n");
	}
	printf("Enjoy the game\n");
	system("pause");
}

void boardPrint(int board[boardSize][boardSize])		//This will clear the screen, then print the current board
{
	int spotColor = 0;		//This will invert for the checker pattern

	system("cls");		//This clears the screen
	printf(" ");		//This is to space out the column identifiers
	for (int columnIdent = 0; columnIdent < boardSize; columnIdent++)		//This prints the column identifiers per boardSize
		printf(" %c", 'A' + columnIdent);		//This prints the column identifiers
	printf("\n");		//This is the end of the column identifiers
	for (int row = 0; row < boardSize; row++)		//This runs for each row
	{
		printf("%d ", row + 1);		//This prints the row identifiers
		for (int column = 0; column < boardSize; column++)		//This runs for each column
		{
			switch (board[row][column])		//This looks at a specific spot and prints its representation on the printed board
			{
			case emptySpot:
				if (spotColor == 0)		//This will print the checker pattern
					printf("\x1b[7m %c\x1b[0m", ' ');
				else
					printf("\x1b[0m %c\x1b[0m", ' ');
				break;
			case player1Pawn:
				printf("\x1b[0m %c\x1b[0m", 'b');
				break;
			case player1King:
				printf("\x1b[0m %c\x1b[0m", 'B');
				break;
			case player2Pawn:
				printf("\x1b[0m %c\x1b[0m", 'r');
				break;
			case player2King:
				printf("\x1b[0m %c\x1b[0m", 'R');
				break;
			default:		//This should never happen, if it does then it will print an error
				printf("\x1b[31m!Error: Invalid value at %d%c, has value %d!\x1b[0m\n", row + 1, column + 65, board[row][column]);
			}
			if (spotColor == 0 && (column + 1 != boardSize || boardSize % 2 == 1))		//This invert spotColor as long as its not on an edge with a odd boardSize
				spotColor = 1;
			else if (spotColor == 1 && (column + 1 != boardSize || boardSize % 2 == 1))
				spotColor = 0;
		}
		printf("\n");		//This will print a new line for the next row
	}
}

int selector(int *rowR, int *columnR)		//This will allow the user to enter coordinates on the board
{
	char row;		//This holds the input for row
	char column;		//This holds the input for column
	int done = FALSE;		//The selector loop will continue as long as this is false
	int returnState = 0;		//This will normally be 0, unless the user asks to go back, in which case it will be 1

	while (!done)		//This will loop untill the user enters valid coordinates on the board
	{
		scanf(" %c", &row);
		scanf(" %c", &column);
		while (getchar() != '\n');		//This wipes the buffer to prevent errors
		if (isalpha(row) && isdigit(column))		//This will swap row and column if the user enters column first
		{
			char temp = row;
			row = column;
			column = temp;
		}
		column = toupper(column);		//If the user enters a lowercase letter this will capitalize it
		if (row == '0' && column == 'Q')		//If the user enters 0Q, then the will leave selector() and go back a stage or quit
		{
			returnState = 1;
			done = TRUE;
		}
		else if (row > '0' + boardSize || row < '1')		//This tests that row is valid
			printf("Error: Row out of range\n");
		else if (column > 'A' + boardSize || column < 'A')		//This tests that column is valid
			printf("Error: column out of range\n");
		else		//If the input is good it will convert them to ints and return to main
		{
			*rowR = row - '1';
			*columnR = column - 'A';
			done = TRUE;
		}
	}
	return returnState;		//This will normally be 0, unless the user asks to go back, in which case it will be 1
}

int moveAttempt(int board[boardSize][boardSize], int rowCurrent, int columnCurrent, int rowTarget, int columnTarget, int aggressive)		//This checks if a move is valid and if so enacts the move
{
	int moveState = moveNormal;		//This holds what will be returned, 0 for a valid non-aggressive move, 1 for a valid aggressive move, and 2 for a invalid move
	switch (board[rowCurrent][columnCurrent])		//This looks at a chip type, so it can follow its specific rules
	{
	case player1Pawn:
		if ((columnTarget == columnCurrent - 1 || columnTarget == columnCurrent + 1) && (rowTarget == rowCurrent - 1) && (aggressive == FALSE))		//This looks to see if target spot is under the current chip and only one spot horizontally away			//This is NOT player relative, (rowTarget == rowCurrent - 1) must go up on board
		{
			printf("Good Move\n");
			moveState = moveNormal;
		}
		else if ((columnTarget == columnCurrent - 2 || columnTarget == columnCurrent + 2) && (rowTarget == rowCurrent - 2))		//This looks to see if target spot is under the current chip and only two spots horizontally away			//This is NOT player relative, (rowTarget == rowCurrent - 2) must go up on board
		{
			if (columnCurrent < columnTarget && board[rowCurrent - 1][columnCurrent + 1] < emptySpot)		//This looks to see if player2s chip is between the current chip and the target spot			//This is NOT player relative, (board[rowCurrent - 1][columnCurrent + 1] < emptySpot) must be player2 chip
			{
				board[rowCurrent - 1][columnCurrent + 1] = emptySpot;		//This removes the other players chip			//This is NOT player relative, [rowCurrent - 1] must go up on board
				printf("Killer Move\n");
				moveState = moveAggressive;
			}
			else if (columnCurrent > columnTarget && board[rowCurrent - 1][columnCurrent - 1] < emptySpot)		//This looks to see if other players chip is between the current chip and the target spot			//This is NOT player relative, (board[rowCurrent - 1][columnCurrent - 1]) < 0) must be player2 chip
			{
				board[rowCurrent - 1][columnCurrent - 1] = emptySpot;		//This removes the other players chip			//This is NOT player relative, [rowCurrent - 1] must go up on board
				printf("Killer Move\n");
				moveState = moveAggressive;
			}
			else		//This happens if the other players chip is missing
			{
				printf("Error: Missing Enemy\n");
				moveState = moveInvalid;
			}
		}
		else		//This happens if target is not valid or if the player trys a non-aggressive move after an aggressive move
		{
			printf("Error: Invalid Location\n");
			moveState = moveInvalid;
		}
		break;
	case player2Pawn:
		if ((columnTarget == columnCurrent - 1 || columnTarget == columnCurrent + 1) && (rowTarget == rowCurrent + 1) && (aggressive == FALSE))			//This looks to see if target spot is over the current chip and only one spot horizontally away			//This is NOT player relative, (rowTarget == rowCurrent + 1) must go down on board
		{
			printf("Good Move\n");
			moveState = moveNormal;
		}
		else if ((columnTarget == columnCurrent - 2 || columnTarget == columnCurrent + 2) && (rowTarget == rowCurrent + 2))		//This looks to see if target spot is over the current chip and only two spots horizontally away			//This is NOT player relative, (rowTarget == rowCurrent + 2) must go down on board
		{
			if (columnCurrent < columnTarget && board[rowCurrent + 1][columnCurrent + 1] > emptySpot)		//This looks to see if player1s chip is between the current chip and the target spot			//This is NOT player relative, (board[rowCurrent - 1][columnCurrent + 1]) > 0) must be player1 chip
			{
				board[rowCurrent + 1][columnCurrent + 1] = emptySpot;		//This removes the other players chip			//This is NOT player relative, [rowCurrent + 1] must go down on board
				printf("Killer Move\n");
				moveState = moveAggressive;
			}
			else if (columnCurrent > columnTarget && board[rowCurrent + 1][columnCurrent - 1] > emptySpot)		//This looks to see if other players chip is between the current chip and the target spot			//This is NOT player relative, (board[rowCurrent - 1][columnCurrent + 1]) > 0) must be player1 chip
			{
				board[rowCurrent + 1][columnCurrent - 1] = emptySpot;		//This removes the other players chip			//This is NOT player relative, [rowCurrent + 1] must go down on board
				printf("Killer Move\n");
				moveState = moveAggressive;
			}
			else		//This happens if the other players chip is missing
			{
				printf("Error: Missing Enemy\n");
				moveState = moveInvalid;
			}
		}
		else		//This happens if target is not valid or if the player trys a non-aggressive move after an aggressive move
		{
			printf("Error: Invalid Location\n");
			moveState = moveInvalid;
		}
		break;
	case player1King:
	case player2King:
		if (((rowTarget == rowCurrent + 1) || (rowTarget == rowCurrent - 1)) && ((columnTarget == columnCurrent - 1) || (columnTarget == columnCurrent + 1)) && (aggressive == FALSE))			//This looks to see if target spot is only one spot horizontally away in any quadrant			//This is player relative
		{
			printf("Good Move\n");
			moveState = moveNormal;
		}
		else if (((rowTarget == rowCurrent + 2) || (rowTarget == rowCurrent - 2)) && ((columnTarget == columnCurrent - 2) || (columnTarget == columnCurrent + 2)))		//This looks to see if target spot is over the current chip and only two spots horizontally away
		{
			if (rowCurrent < rowTarget && columnCurrent < columnTarget)		//This looks to see if target spot is in the 4th quadrant
			{
				if (board[rowCurrent][columnCurrent] < emptySpot)		//This looks to see if currrent chip is for player2
				{
					if (board[rowCurrent + 1][columnCurrent + 1] > emptySpot)		//This looks to see if player1s chip is between the current chip and the target spot
					{
						board[rowCurrent + 1][columnCurrent + 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
				else		//This looks to see if currrent chip is for player1
				{
					if (board[rowCurrent + 1][columnCurrent + 1] < emptySpot)		//This looks to see if player2s chip is between the current chip and the target spot
					{
						board[rowCurrent + 1][columnCurrent + 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
			}
			else if (rowCurrent < rowTarget && columnCurrent > columnTarget)		//This looks to see if target spot is in the 3rd quadrant
			{
				if (board[rowCurrent][columnCurrent] < emptySpot)		//This looks to see if currrent chip is for player2
				{
					if (board[rowCurrent + 1][columnCurrent - 1] > emptySpot)		//This looks to see if player1s chip is between the current chip and the target spot
					{
						board[rowCurrent + 1][columnCurrent - 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
				else		//This looks to see if currrent chip is for player1
				{
					if (board[rowCurrent + 1][columnCurrent - 1] < emptySpot)		//This looks to see if player2s chip is between the current chip and the target spot
					{
						board[rowCurrent + 1][columnCurrent - 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
			}
			else if (rowCurrent > rowTarget && columnCurrent < columnTarget)		//This looks to see if target spot is in the 1st quadrant
			{
				if (board[rowCurrent][columnCurrent] < emptySpot)		//This looks to see if currrent chip is for player2
				{
					if (board[rowCurrent - 1][columnCurrent + 1] > emptySpot)		//This looks to see if player1s chip is between the current chip and the target spot
					{
						board[rowCurrent - 1][columnCurrent + 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
				else		//This looks to see if currrent chip is for player1
				{
					if (board[rowCurrent - 1][columnCurrent + 1] < emptySpot)		//This looks to see if player2s chip is between the current chip and the target spot
					{
						board[rowCurrent - 1][columnCurrent + 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
			}
			else		//This looks to see if target spot is in the 2nd quadrant
			{
				if (board[rowCurrent][columnCurrent] < emptySpot)		//This looks to see if currrent chip is for player2
				{
					if (board[rowCurrent - 1][columnCurrent - 1] > emptySpot)		//This looks to see if player1s chip is between the current chip and the target spot
					{
						board[rowCurrent - 1][columnCurrent - 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
				else		//This looks to see if currrent chip is for player2
				{
					if (board[rowCurrent - 1][columnCurrent - 1] < emptySpot)		//This looks to see if player2s chip is between the current chip and the target spot
					{
						board[rowCurrent - 1][columnCurrent - 1] = emptySpot;		//This removes the other players chip
						printf("Killer Move\n");
						moveState = moveAggressive;
					}
					else		//This happens if the other players chip is missing
					{
						printf("Error: Missing Enemy\n");
						moveState = moveInvalid;
					}
				}
			}
		}
		else		//This happens if target is not valid or if the player trys a non-aggressive move after an aggressive move
		{
			printf("Error: Invalid Location\n");
			moveState = moveInvalid;
		}
		break;
	default:
		printf("Error: Unknown piece, has value %d at %d%c\n", board[rowCurrent][columnCurrent], rowTarget + 1, columnTarget + 'A');
		moveState = moveInvalid;
	}
	if (moveState != 2)
	{
		board[rowTarget][columnTarget] = board[rowCurrent][columnCurrent];
		board[rowCurrent][columnCurrent] = 0;
	}
	return moveState;
}

void chipCounter(int board[boardSize][boardSize], int *player1ChipC, int *player2ChipC)		//This counts the chips for both players
{
	*player1ChipC = 0;		//sets player1ChipC to 0 for the new count
	*player2ChipC = 0;		//sets player2ChipC to 0 for the new count
	for (int row = 0; row < boardSize; row++)		//This runs for each row
	{
		for (int column = 0; column < boardSize; column++)		//This runs for each column
		{
			switch (board[row][column])		//This looks at a specific spot and if its a chip it counts if for that player
			{
			case emptySpot:
				break;
			case player1Pawn:
			case player1King:
				*player1ChipC = *player1ChipC + 1;
				break;
			case player2Pawn:
			case player2King:
				*player2ChipC = *player2ChipC + 1;
				break;
			default:		//This should never happen, if it does then it will print an error
				printf("\n\x1b[31m!Error: Invalid value at %d%c, has value %d!\x1b[0m\n", row + 1, column + 65, board[row][column]);
			}
		}
	}
	printf("Player1:%d Player2:%d\n", *player1ChipC, *player2ChipC);		//This prints the chip count for the user
}
