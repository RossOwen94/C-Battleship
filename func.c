 
#include "prgm.h"



/*
===================================================================================================
Function	: print_game_menu
Parameters	: none
Return		: integer, the number of shots allowed
Description	: Prints the game instructions. Calls set_difficulty to parse user input difficulty
===================================================================================================
*/
int print_game_menu(void)
{
	int allowed_attempts;	
	
	printf("\n\nWelcome to Battleship!");
	set_difficulty(&allowed_attempts);
	printf("\n\n+------------------------------------------------------------+\n");
	printf("| The computer program will randomly assign an aircraft car- |\n");
	printf("| rier and a battleship, that are oriented either vertically |\n");
	printf("| or horizontally, to the board. You will have %d chances to |\n",allowed_attempts);
	printf("| sink both the computer's aircraft carrier and battleship!! |\n");
	printf("| You'll play on a %d X %d board, where the aircraft carrier |\n",BOARD_SIZE,BOARD_SIZE);
	printf("| will have a length of 5 units and the battleship will have |\n");
	printf("| a length of 4 units.                                       |\n");
	printf("+------------------------------------------------------------+\n\n\n");
	
	return allowed_attempts;
}

/*
==================================================================================
Function	: set_difficulty
Parameters	: pointer to integer, allowed_attempts from caller print_game_menu
Return		: none, changes contents of pointer
Description	: parses the user inputted difficulty string
==================================================================================
*/
void set_difficulty(int *allowed_attempts)
{
	char difficulty[100];
	
	
	printf("\n\nEnter difficulty level of game(easy,normal,hard): ");
	gets(difficulty);

	/*Loops until proper input is received*/
	while((strcmp(difficulty,"easy")!=0)&&(strcmp(difficulty,"normal")!=0)&&(strcmp(difficulty,"hard")!=0))
	{
		printf("\nYou did not enter (easy, normal, or hard)!\n");
		printf("Please re-enter the difficulty:: \n");
		gets(difficulty);
	}
	 
	 /*Easy*/
	if(strcmp(difficulty,"easy")==0)
		*allowed_attempts=30;
	else/*Normal*/
	   if(strcmp(difficulty,"normal")==0)
	   	   *allowed_attempts=25;
	   else/*Hard*/
	      *allowed_attempts=20;

	printf("\nDifficulty set to:: %s\n",difficulty);
	return ;
}

/*
======================================================================================================
Function	: initialize_board
Parameters	: Gameboard data structure, integer board size, string of ship being placed
Return		: none
Description	: Will attempt to place a ship until proper placement is found. Will first check
		  if the ship falls off the end of the board. Will then call check_for_overlap
		  to see if it was placed on top of another ship. If it meets all of these requirements
		  the ship will be assigned to the corresponding elements in the data structure
=======================================================================================================
*/
void initialize_board(char **board,int board_size,char *ship_to_be_placed)
{	
	int i;
	int x_max;
	int y_max;
	int vertical;
	int x;
	int y;
	int pieces_fit=0;
	int overlap_check;

	printf("Placing:: %s... .. . \n\n",ship_to_be_placed);
	do{
		/*These initializations will be repeated if they do not fit the parameters*/
		/**************************************************************************/
		 vertical=rand()%2;/*1 equals vertical,0 equals horizontal*/
		 x=rand()%board_size;/*starting x co-ord*/
		 y=rand()%board_size;/*starting y co-ord*/
		 x_max=x;/*ending x co-ord*/
		 y_max=y;/*ending y co-ord*/
		 overlap_check=1;


	/*Calculate the end position of the ship*/
	/****************************************/
	if(vertical)/*Vertical*/	
        {
		if(strcmp(ship_to_be_placed,"battleship")==0)
			y_max+=3;
		else
			y_max+=4;
	}
	else/*Horizontal*/
	{
		if(strcmp(ship_to_be_placed,"battleship")==0)
			x_max+=3;/*Battleships have a length of 4*/
		else
			x_max+=4;/*Carriers have a length of 5*/
	}


	/*Do they go past end of board?*/
	/*******************************/
	if(y_max<board_size&&x_max<board_size)
		pieces_fit=1;
	
	/*If the pieces do not run off the ends of the board, do they overlap with a previously placed piece?*/
	if(pieces_fit==1)
	{	if(vertical)
			overlap_check=check_for_overlap(board,x,y,y_max,vertical);
		else
			overlap_check=check_for_overlap(board,x,y,x_max,vertical);
	}

	/*If pieces fit on the board but are found to overlap with another piece*/
	if(overlap_check==1)
		pieces_fit=0;/*Then they no longer fit on the board*/
	
	
	}while(pieces_fit==0);/*If pieces_fit is 1, it is OK to exit the loop and place them on the board*/
	



	/*This remaining section handles the assignment of the ships to the data structure*/
	/*All previous code has simply checked for the suitability of their position*/
	/****************************************************************************/
	if(vertical)
	{	
		if(strcmp(ship_to_be_placed,"battleship")==0)
			for(i=y;i<=y_max;i++)/*iterates through the ships positions*/
				board[i][x]='B';/*assigns 'B' for battleship*/
		else
			for(i=y;i<=y_max;i++)
				board[i][x]='A';/*assigns 'A' for aircraft carriers*/
	}
	else/*Same as the above, only horizontal*/
	{
		if(strcmp(ship_to_be_placed,"battleship")==0)
			for(i=x;i<=x_max;i++)
				board[y][i]='B';
		else
			for(i=x;i<=x_max;i++)
				board[y][i]='A';
	}

	return ;
} 

/*
=========================================================================================================
Function	: check_for_overlap
Parameters	: Gameboard data structure,x & y coords of ship, x or y max of ship and it's direction
Return		: integer, 1 (true: there is overlap), 0 (false: no overlap)
Description	: This function will check if there is already a ship where the caller 'initialize_board' 
		  is attempting to place a ship. 
=========================================================================================================
*/
int check_for_overlap(char **board,int x, int y, int vector_max,int vertical)
{
	int i;

	if(vertical)
		for(i=y;i<=vector_max;i++)
		{
			if(board[i][x]=='A'||board[i][x]=='B')/*If there is an 'A' or 'B' there is a ship there*/
				return 1;/*Indicates overlap*/
			
		}
	else/*Horizontal*/
		for(i=x;i<=vector_max;i++)
		{
			if(board[y][i]=='A'||board[y][i]=='B')
				return 1;

		}

	return 0;/*Indicates no overlap*/

}

/*
============================================================================================================
Function	: print_board
Parameters	: Gameboard data structure, integer board size, integer reveal(0 or 1)
Return		: none
Description	: This function traverses the entire game board and prints it out. If reveal 
		  is set to 0 it will not show ship locations. If reveal is set to 1 it will show everything
============================================================================================================
*/
void print_board(char **board,int board_size,int reveal,int attempts_allowed)
{
	int i,j;

	if(reveal==0)
	{
		for(i=0;i<board_size;i++)
			printf("-------");
		printf("\n|      |");
		for(i=1;i<=board_size;i++)
		{					/************************/
			if(i!=10)			/*This block formats the*/
				printf("  %d  |",i);	/*output to look like   */
			else				/*a table in the console*/
				printf("  %d |",i);     /************************/
		}
		printf("\n");
		for(i=0;i<board_size;i++)
			printf("-------");
		printf("\n");
	

		/*The following iterates through the board and prints out everything but ship locations*/
		for(i=0;i<board_size;i++)
		{	printf("|  %c   |",i+65);
			for(j=0;j<board_size;j++)
			{
				if(board[i][j]=='B'||board[i][j]=='A')
						printf("     |");/*Does not show 'A' or 'B' */	
				else
				    if(board[i][j]=='O' || board[i][j]=='X')
					printf("  %c  |",board[i][j]);/*Will display 'X' and 'O' */
				    else
					printf("  %c   |",board[i][j]);/*Prints empty spots as spaces*/
			}
			printf("\n");
			for(j=0;j<board_size;j++)
				printf("-------");
			printf("\n");
		}
	}
	else/*Reveal is equal to 1(Will show everything)*/
	{	
		for(i=0;i<board_size;i++)
			printf("-------");
	        printf("\n|      |");
		for(i=1;i<=board_size;i++)
		{					/****************************/
			if(i!=10)			/*Again, this block formats*/
				printf("  %d  |",i);	/*the output to be a table*/
			else				/*in the console window  */
				printf("  %d |",i);	/************************/
		}
		printf("\n");
		for(i=0;i<board_size;i++)
			printf("-------");
		printf("\n");
	

		/*The following block iterates through the board and shows everything*/
		for(i=0;i<board_size;i++)
		{	printf("|  %c   |",i+65);
			for(j=0;j<board_size;j++)
			{
				if(board[i][j]=='B'||board[i][j]=='A' || board[i][j]=='O' || board[i][j]=='X')
						printf("  %c  |",board[i][j]);/*Will show contents of board*/	
				else
					printf("  %c   |",board[i][j]);
			}
			printf("\n");
			for(j=0;j<board_size;j++)
				printf("-------");
			printf("\n");
		}
	}
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


	return ;

} 



/*
===================================================================================================================
Function	: fire_shot
Parameters	: The gameboard data structure, the structure for handling torpedo shots
Return		: none
Description	: This function will change the data structure in accordance with the shots the user fires.
		  Will place 'X' or 'O' for hits or misses, respectively, and will increment the hits for each ship
		  so as to know when they have sunk.
===================================================================================================================
*/
void fire_shot(char**board,struct active_board *shot_tracker)
{
	int x,y;
	int duplicate_shot;
	x=shot_tracker->column-1;
	y=shot_tracker->row;
	char row_input;

		if(board[y][x]=='A' || board[y][x]=='B')
		{	
			printf("HIT!...NICE SHOT!\n");
			if(board[y][x]=='A')
		    	{	shot_tracker->carrier_hits+=1;
				if(shot_tracker->carrier_hits==5)
					printf("Congratulations! You sank my Aircraft Carrier!\n");
			}
			else
			{	shot_tracker->battleship_hits+=1;
				if(shot_tracker->battleship_hits==4)
					printf("Congratulations! You sank my Battleship!\n");
			}
			board[y][x]='X';
		}
	

		else
		{
		  if(board[y][x]=='O' || board[y][x]=='X')
		   {
				printf("WASTED SHOT!!!! You have already shot at that position!\n");
				if(board[y][x]=='O')
					printf("Previous shot resulted in a:: MISS...\n");
				else
					printf("Previous shot resulted in a::  HIT!\n");
		   }

	           else
		   {	board[y][x]='O';
			printf("Miss...\n");
		   }
		} 

	return ;
}

/*
===============================================================================================================================
Function	: save_game
Parameters	: A File pointer to the current game as well as all status variables subject to change
		  during gameplay
Return		: none
Description	: Prints each variable to the file pointed to by current_game. Will save the file with the same organization as 
                  the load_game function. Saves as a text file, not binary. DO NOT CHEAT!
===============================================================================================================================
*/
void save_game(FILE *current_game, char **board, int board_size,int allowed_attempts,struct active_board *shot_tracker, int i)
{
	int j,k;

	for(j=0;j<board_size;j++)
	{	
		for(k=0;k<board_size;k++)
		{	fprintf(current_game,"%c",board[j][k]);
		}		
		fprintf(current_game,"\n");
	}

	fprintf(current_game,"\n");
	fprintf(current_game,"%d\n", board_size);
	fprintf(current_game,"%d\n",allowed_attempts);
	fprintf(current_game,"%d %d\n",shot_tracker->carrier_hits,shot_tracker->battleship_hits);
	fprintf(current_game,"%d",i);
}

/*
=============================================================================================================================
Function	: load_game
Parameters	: A File pointer to the current game as well as POINTERS to all status variables subject to change
		  during gameplay.
Return		: none
Description	: Will scan from the file pointed to by saved_game. Procedure is identical to that of the save_game function.
		  Uses pointers to affect the variables in the calling environment main()
=============================================================================================================================
*/
void load_game(FILE *saved_game,char **board,int *board_size, int *allowed_attempts,struct active_board *shot_tracker, int *i)
{
	int j,k;
	
	for(j=0;j<BOARD_SIZE;j++)
	{
		for(k=0;k<BOARD_SIZE;k++)
		{
			fscanf(saved_game,"%c",&board[j][k]);
		}
		fgetc(saved_game);
	}

	fscanf(saved_game,"%d",board_size);
	fscanf(saved_game,"%d",allowed_attempts);
	fscanf(saved_game,"%d %d",&shot_tracker->carrier_hits,&shot_tracker->battleship_hits);
	fscanf(saved_game,"%d", i);
}
