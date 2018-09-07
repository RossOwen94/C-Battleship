
 
    
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10

struct active_board{
	int row;
	int column;
	int carrier_hits;
	int battleship_hits;
	};

void print_my_info(void);
int print_game_menu(void);
void set_difficulty(int*);
void initialize_board(char**,int,char*);
void print_board(char**,int,int,int);
int check_for_overlap(char **,int,int,int,int);
void fire_shot(char**,struct active_board *);
void save_game(FILE*,char**,int,int,struct active_board *,int);
void load_game(FILE*,char**,int*,int*,struct active_board *,int *);



