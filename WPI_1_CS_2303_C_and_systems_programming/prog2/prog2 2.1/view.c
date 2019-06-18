#include "header.h"

//Ezra Davis
//Homework assignment #2
//CS2303

//Ezra Davis
//Returns the direction of the player as a string
char *getDirection(Player player){
	switch(player.direction){
		case West:
		return "West ";
		case East:
		return "East ";
		case North:
		return "North";
		case South:
		return "South";
	}
	return "Error, improper direction";
}

//Ezra Davis
//prints players names & location
void printPlayers(Player* field){
	unsigned short i;//counter

	printf("Player:\t| Location: | Direction: |\n");
	
	for(i=0; i < 2 * TEAMSIZE; i++){
		if(field[i].alive){
			printf(" %c%1d\t| %3hux%3hu   | %5s      |\n",
				field[i].team, field[i].number, field[i].row, field[i].column, getDirection(field[i]));
		}
		
	}
}//end printPlayers

//Ezra Davis
//Prints the player icon at the row and column, or "  " (only 2 ascii characters long)
void printPlayerIcon(Player* field, unsigned short row, unsigned short column){
	unsigned int i;//Counter
	for(i=0; i < TEAMSIZE * 2; i++){
		if(field[i].row == row && field[i].column == column && field[i].alive){
			
			printf("%c%1d", field[i].team, field[i].number % 10); //Prints out only last digit of the player's number if there is more than 9 players per team
			return;
		}//end if
	}//end for
	
	printf("  ");
	
	return ;
}//end findPlayer

//Ezra Davis
//Prints the smashball field
void printField(Player* field){

	unsigned short i;//counter for columns
	unsigned short j;//counter for rows
	
	//prints the top border of the field
	for(j=0; j < WIDTH; j++){
		printf("---");
	}
		printf("---\n");
	
	//loops through rows
	for(i=0; i < HEIGHT; i++){
		printf("|");
		
		//loops through columns
		for(j=0; j < WIDTH; j++){
			//Prints players at this location
			printf(" ");
			printPlayerIcon(field, i, j);
		}
		printf(" |\n");
	}
	
	//prints the top border of the field
	for(j=0; j < WIDTH; j++){
		printf("---");
	}
		printf("---\n");
}//end printField