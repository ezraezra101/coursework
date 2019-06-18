#include "header.h"

//Ezra Davis
//Homework assignment #2
//CS2303

//Ezra Davis
//returns true if the player is on another team, and kills the player hopped onto, otherwise returns false
bool hopOnto(Player* field, char hopperTeam, unsigned short row, unsigned short column){
	
	if(VERBOSE){
		printf("Player of team %c is hopping onto space %hux%hu\n", hopperTeam, row, column);
	}
	
	//Finds the player being hopped onto
	unsigned short i; //Counter
	for(i=0; i< 2*TEAMSIZE; i++){
		if(field[i].row == row && field[i].column == column && field[i].alive){
			break;
		}//end if
	}//end for
	
	if(field[i].team == hopperTeam){
		
		if(VERBOSE){
			printf("Player %c%hu has been hopped over\n", field[i].team, field[i].number);
		}
		
		return false;
	}
	else {//players on opposite team
		
		if(VERBOSE){
			printf("Player %c%hu has been crushed\n", field[i].team, field[i].number);
		}
		
		field[i].alive = false;
		return true;
	}
}//end hopOnto

//Ezra Davis
//Places a player and determines consequences of new placement
void placePlayer(Player* field, Player* activePlayer, unsigned short rowTo, unsigned short columnTo){
	//if on edge, bounce
	if(rowTo < 0 || rowTo >= HEIGHT || columnTo < 0 || columnTo >= WIDTH){
		
		//turn around
		switch(activePlayer->direction){
			case West:
				activePlayer->direction = East;
				break;
			case East:
				activePlayer->direction = West;
				break;
			case North:
				activePlayer->direction = South;
				break;
			case South:
				activePlayer->direction = North;
				break;
		}//end switch
		
		movePlayer(field, activePlayer);//try moving again
		return;
	}		
	//if there isn't a collision
	else if(!checkCollision(field, rowTo, columnTo)){
		activePlayer->row = rowTo;
		activePlayer->column = columnTo;
		return;
	}
	//If the player hopped onto the same team
	else if(!hopOnto(field, activePlayer->team, rowTo, columnTo)){
		//then go again! (effectively hopping over any teammates)
		activePlayer->row = rowTo;
		activePlayer->column = columnTo;
		movePlayer(field, activePlayer);
		return;
	}
	//If the activePlayer hits a player on the other team
	else {
		activePlayer->row = rowTo;
		activePlayer->column = columnTo;
		//hopOnto has already smashed the other player
		return;
	}

}//end placePlayer

//Ezra Davis
//Moves a single player
void movePlayer(Player* field, Player* activePlayer){
	
	if(VERBOSE){
		printf("Moving player %c%1d on the field in the %d direction\n", activePlayer->team, activePlayer->number, activePlayer->direction);
	}
	
	if(activePlayer->alive == false){
		//If the player has been hit, don't move
		if(VERBOSE){
			printf("Nevermind, the player has been hit & can't move.\n");			
		}
		return;
	}
	else if(activePlayer->direction == North){
		//move North
		placePlayer(field, activePlayer, activePlayer->row - 1, activePlayer->column);
		
	}
	else if(activePlayer->direction == East){
		//move East
		placePlayer(field, activePlayer, activePlayer->row, activePlayer->column + 1);
	}
	else if(activePlayer->direction == South){
		//move South
		placePlayer(field, activePlayer, activePlayer->row + 1, activePlayer->column);
	}
	else { //activePlayer->direction == West
		//move West
		placePlayer(field, activePlayer, activePlayer->row, activePlayer->column - 1);
	}//end direction choice ifs
} //end movePlayer

//Ezra Davis
//Randomizes turn order
void randomizeTurnOrder(Player* field){
	if(VERBOSE){
		printf("Randomizing turn order");
	}
	
	/*Goes through picking a random player from the rest of the list
	to fill the first remaining unrandomized spot (not the algorithm we saw in class)*/
	
	Player swap;//used as a temp value during switching of players
	int i; //holds place in list of turns
	int randomLocation; //set to random locations left in the deck
	
	for (i=0; i < TEAMSIZE * 2; i++){
		//sets randomLocation to a number between i and the subscript of the last player on the field (i to TEAMSIZE * 2)
		randomLocation = rand() % (TEAMSIZE * 2 - i) + i;
		
		swap = field[i];
		field[i] = field[randomLocation];
		field[randomLocation] = swap;
	}//end for
}// end randomizeTurnOrder

//Ezra Davis
//Runs a single round of SmashBall. Returns teamName if a team has won. Otherwise, returns '\0'
char runRound(Player* field){
	if(VERBOSE){
		printf("Running a round!\n");
	}

	//@@ make turn ordering fair
	unsigned short i; //Player Counter
	for(i=0; i < TEAMSIZE*2; i++){
		movePlayer(field, &field[i]);
		
		if(winner(field) != '\0'){
			return winner(field);
		}
	}
	
	return '\0';
}//end runRound

//Ezra Davis
//plays a game of smashball
int main(){
	srand (time(NULL));
	Player* field = fieldInitialize(TEAM1, TEAM2);
	
	//Determines if the turn order is randomized or not.
	if(RANDOMIZEDTURNORDER){
		randomizeTurnOrder(field);
	}

	
	printPlayers(field);
	
	unsigned short turnCount = 0; //Counts rounds
	char win = '\0'; //Determines winner, if '\0', then the game hasn't terminated early
	
	//Runs the game until an end condition is met
	while((turnCount < MAXROUNDS) && (win == '\0')){
		
		//Prints the field every TURNSBETWEENPRINTS turns
		if( turnCount % TURNSBETWEENPRINTS == 0 || VERBOSE){
			printf("Turn: %d\n", turnCount);
			printField(field);
			
			if(VERBOSE){ //prints out players in debug mode
				printPlayers(field);
			}

		} //end conditional printing
		
		turnCount++;
		
		win = runRound(field);
		
	}//End while

	//Prints out end of match info
	printf("Turn: %d\n", turnCount);
	printField(field);
	printPlayers(field);
	if(win == '\0'){
		printf("What an exciting draw!\n");
		printf("%hu members of team A survived.\n", countTeammembers(field,'A'));
		printf("%hu members of team B survived.\n", countTeammembers(field,'B'));
	}
	else {
		printf("Team %c wins!\n", win);
	}

	return 0;
}//end main
