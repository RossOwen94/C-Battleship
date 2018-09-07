
#include "prgm.h"


int main(void)
{	
	srand(time(NULL));			/*Seeds the pseudo-random # generator*/
	char **board;				/*Will store the gameboard data*/
	int allowed_attempts;			/*Total # of shots the user is allowed*/
	int board_size=BOARD_SIZE;		/*The size of the board*/
	char ship_to_be_placed[20];		/*The user will input which ship to place first*/
	char *second_ship;			/*Program will automatically assign the other ship*/
	int i;					/*Counter for outter loops and shot #*/
	int reveal=0;				/*Modifies functionality of print_board()  */
	struct active_board shot_tracker;	/*Stores x,y coords of shot as well as ship hits*/
	int x;					/*x coord of torpedo selection*/
	int y;					/*y coord of torpedo selection*/
	char row_input;				/*User will input a char for the row(will convert to int)*/
	int badInput=0;				/*Status variable for bad input*/
	char resume_game;			/*boolean 'y' or 'n' to load saved game*/
	FILE *current_game;			/*Pointer to a file to save the game*/
	int c;					/*Used to catch input to clear buffer*/




	/*Initializes the array as an array of pointers*/
	board=malloc(BOARD_SIZE*sizeof(char *));
	for(i=0;i<BOARD_SIZE;i++)
	{	/*Creates an array for each pointer resulting in a 10*10 matrix*/
		board[i]=malloc(BOARD_SIZE*sizeof(char *));
	}


	
	/*Loading the game*/
	/******************/

	printf("Would you like to resume the previously saved game(y or n)::  ");
	scanf("%c",&resume_game);
	while(resume_game!='y' && resume_game!='n')/*Input check*/
	{	getchar();
		printf("Error. Enter 'y' or 'n' to load a saved game or not:: ");
		scanf("%c",&resume_game);	
	}
	getchar();
	putchar('\n');
	if(resume_game=='y')
	{
		FILE *saved_game;
		saved_game=fopen("battleship_save","r");/*Will always load from the same filename*/
		if(saved_game==NULL)			/*So there can only be one save at a time*/
		{
			printf("Sorry, no saved game was found. The game will start from the beginning\n");
		}
		else
		{
			load_game(saved_game,board,&board_size,&allowed_attempts,&shot_tracker,&i);
			fclose(saved_game);
			printf("Loading game...\n\n");
			printf("Game loaded succesfully!\n\n");
			print_board(board,board_size,reveal,allowed_attempts);
			goto gamePlay;/*To skip the initialization of the game*/
		}
	}

	allowed_attempts=print_game_menu();

	printf("Enter which ship to place first('battleship', or 'aircraft carrier': ");
	gets(ship_to_be_placed);
	printf("\n\n");

	/*Will loop until proper input is received*/
	while(strcmp(ship_to_be_placed,"battleship")!=0&&strcmp(ship_to_be_placed,"aircraft carrier")!=0)
	{
		printf("You did not input 'battleship' or 'aircraft carrier'.\n");
		printf("Please input the ship to be placed again:: ");
		gets(ship_to_be_placed);	
	}

	
	initialize_board(board,board_size,ship_to_be_placed);

	/*The following if-else allows the user to choose which ship is placed first*/
	if(strcmp(ship_to_be_placed,"battleship")==0)
		second_ship="aircraft carrier";
	else
		second_ship="battleship";


	initialize_board(board,board_size,second_ship);			/*Initializes the second ship*/
	print_board(board,board_size,reveal,allowed_attempts);	/*Prints the board after being fully initialized*/
	
	/*These are counters so they need to be initialized*/
	shot_tracker.carrier_hits=0;
	shot_tracker.battleship_hits=0;	

	i=1;			/*The following do-while represents the turn-by-turn gameplay*/		 
gamePlay:do{	

		/*Receiving shot input*/
		/**********************/
		printf("\nEnter position to fire torpedo #%d (e.g., B7): ",i);
		printf("\nShots remaining: %d\n",allowed_attempts);
		scanf("%c",&row_input);
		scanf("%d",&shot_tracker.column);
		getchar();
		shot_tracker.row=row_input-65;/*Conversion using ASCII value of 'A'*/
		while(shot_tracker.row<0 || shot_tracker.row>9 || shot_tracker.column<1 || shot_tracker.column>10)
		{		while((c=getchar())!='\n')
					;
				printf("Sorry, that is not correct input:: please enter");
				printf("A-J and 1-10 to fire a shot.\n");
				printf("Enter position to fire torpedo #%d (e.g., B7): ",i);
				scanf("%c",&row_input);
				scanf("%d",&shot_tracker.column);
				getchar();
				shot_tracker.row=row_input-65;	
		}		

		
		/*Firing the shot*/
		/*****************/ 
		fire_shot(board,&shot_tracker);			   
		if(shot_tracker.carrier_hits==5 && shot_tracker.battleship_hits==4)
		{
			printf("Congratulations! You sank both ships in %d tries!\n", i);
			break;
		}
		print_board(board,board_size,reveal,allowed_attempts);
		i++;
		allowed_attempts--;
		if(allowed_attempts==0)/*Ends the game(w/o saving)*/
		{
			printf("\n\nSorry, you were not able to sink my ships...\n");
			printf("Better luck next time, Cap'n!\n");
		}
		else/*Saves the game*/
		{	
			current_game=fopen("battleship_save","w");
			save_game(current_game,board,board_size,allowed_attempts,&shot_tracker,i);
			fclose(current_game);
		}
	  }while(allowed_attempts!=0);
	
	/*At this point the game is over and we can reveal the board*/
	reveal=1;
	print_board(board,board_size,reveal,allowed_attempts);

	return 0;
}
