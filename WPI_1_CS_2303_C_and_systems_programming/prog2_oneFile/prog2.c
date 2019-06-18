//Things marked with @@ need revision
//Maybe deal with '\0' code smell
//Put in separate files
//Make turn ordering fair

//Ezra Davis
//Homework assignment #2
//CS2303

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//Height and width of the smashball court. - Both of these should be higher than 1
#define HEIGHT 25
#define WIDTH 25
//The longest a smashball game can go on
#define MAXROUNDS 1000
//Size of each smashball team - should be less than HEIGHT * WIDTH
#define TEAMSIZE 9

//Prints debug information when true
#define VERBOSE false

//The direction a player is facing
enum direction {North, East, South, West};

//Each one of these structs represents a character
typedef struct
{
	char team;
	unsigned short number;
	enum direction direction;
	bool alive;
	
	unsigned short row;
	unsigned short column;
} Player;

//Function prototype used in placePlayer
void movePlayer(Player* field, Player* activePlayer);

//Creates a player struct
Player makePlayer(char team, int number, int direction, unsigned short row, unsigned short column){
	
	if(VERBOSE){
		printf("Making player %c%1d, facing direction %d at %hux%hu\n", team, number, direction, row, column);
	}
	
	Player player;
	player.team = team;
	player.number = number;
	player.direction = direction;
	player.alive = true;
	
	player.row = row;
	player.column = column;
	
	return player;
} //end makePlayer

//Checks to see if there is a player at the specified row and column
bool checkCollision(Player* field, unsigned short row, unsigned short column){
	
	if(VERBOSE){
		printf("Checking for a collision at point %hux%hu\n", row, column);
	}
	
	unsigned int i;//Counter
	for(i=0; i < TEAMSIZE * 2; i++){
		if(field[i].row == row && field[i].column == column && field[i].alive){
			return true;
		}//end if
	}//end for
	
	return false;
}//End checkCollision

//Places one of the two teams onto the field
void placeTeam(Player* field, char teamName, unsigned short teamsPlaced){
	
	if(VERBOSE){
		printf("Placing team %c\n", teamName);
	}
	
	unsigned short i;//Counter
	unsigned short row; // Row of the next player
	unsigned short column; //Column of the next player

	//Places a single player on each iteration (unless there is a collision)
	for(i=0; i < TEAMSIZE; i++){
		row = rand() % HEIGHT;
		column = rand() % WIDTH;
		
		//Makes a player if there aren't any collisions
		if(!checkCollision(field, row, column)){
			//Makes a player and points it in a random direction
			field[i+ teamsPlaced*TEAMSIZE] = makePlayer(teamName, i+1, rand()%4, row, column);
		}
		else { //Otherwise, try placing the player again
			i--;
		} //end else
	}//end for

} //end placeTeam

//Sets North the field
Player* fieldInitialize(char teamName1, char teamName2){
	if(VERBOSE){
		printf("Initializing the field for a match between %c and %c\n", teamName1, teamName2);
	}
	//An array of Players
	//Keeps track of all the players and their positions
	Player* field = malloc(TEAMSIZE*2 * sizeof(Player));

	//Player[TEAMSIZE*2] field;
	//Player[HEIGHT][WIDTH] field;

	placeTeam(field, teamName1, 0);
	placeTeam(field, teamName2, 1);
	
	return field;
	
} //end fieldInitialize

//returns true if the player is on another team, and kills the player hopped onto, otherwise returns false
bool hopOnto(Player* field, char hopperTeam, unsigned short row, unsigned short column){
	
	if(VERBOSE){
		printf("Player of team %c is hopping onto space %hux%hu\n", hopperTeam, row, column);
	}
	
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

}


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

// Returns teamName if a team has won, otherwise returns '\0'
char winner(Player* field){
	int i=0;//Counts players
	char win = '\0';
	
	//Finds the first non-smashed player
	while(i<TEAMSIZE*2 && win == '\0'){
		if (field[i].alive){
			win = field[i].team;
		}
		i++;
	}//end while
	
	//Checks if there are any members of the opposite team is alive
	for(i=i; i < TEAMSIZE*2; i++){
		if(field[i].alive && field[i].team != win){
			if(VERBOSE){
				puts("people of both teams are alive!");
			}
			return '\0';
		}
	} //end for
	return win; // Returns '\0' if there are no players left, or the winner
}//End winner

//Runs a single round of SmashBall. Returns teamName if a team has won. Otherwise, returns '\0'
char runRound(Player* field){
	if(VERBOSE){
		printf("Running a round!\n");
	}

	//@@ make turn ordering fair
	unsigned short i; //Counter
	for(i=0; i < TEAMSIZE*2; i++){
		movePlayer(field, &field[i]);
		
		if(winner(field) != '\0'){
			return winner(field);
		}
	}
	
	return '\0';
}

//Counts the surviving members of the team
unsigned short countTeammembers(Player* field, char teamName){
	unsigned short i; //Counts all players
	unsigned short teammates = 0;
	
	for(i=0; i<TEAMSIZE*2; i++){
		//if they are on teamName's team, and still unsmashed
		if(field[i].team == teamName && field[i].alive){
			teammates++;
		}
	}
	return teammates;
}//end countTeammembers

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
}

void printPlayers(Player* field){
	unsigned short i;//counter

	printf("Player: | Location: | Direction: |\n");
	
	for(i=0; i < 2 * TEAMSIZE; i++){
		if(field[i].alive){
			printf(" %c%1d     | %3hux%3hu   | %5s      |\n", field[i].team, field[i].number, field[i].row, field[i].column, getDirection(field[i]));
		}
		
	}
}//end printPlayers

//Prints the player icon at the row and column, or "  "
void printPlayerIcon(Player* field, unsigned short row, unsigned short column){
	unsigned int i;//Counter
	for(i=0; i < TEAMSIZE * 2; i++){
		if(field[i].row == row && field[i].column == column && field[i].alive){
			
			printf("%c%1d", field[i].team, field[i].number);
			return;
		}//end if
	}//end for
	
	printf("  ");
	
	return ;
}//end findPlayer

void printField(Player* field){

	unsigned short i;//counter for columns
	unsigned short j;//counter for rows
	
	//prints the top border of the field
	for(j=0; j < WIDTH; j++){
		printf("----");
	}
		printf("--\n");
	
	//loops through rows
	for(i=0; i < HEIGHT; i++){
		printf("|");
		
		//loops through columns
		for(j=0; j < WIDTH; j++){
			//Prints players at this location
			printf(" ");
			printPlayerIcon(field, i, j);
			printf(" ");
		}
		printf("|\n");
	}
	
	//prints the top border of the field
	for(j=0; j < WIDTH; j++){
		printf("----");
	}
		printf("--\n");
}//end printField

//plays a game of smashball
int main(){
	srand (time(NULL));
	Player* field = fieldInitialize('A', 'B');
	
	printPlayers(field);
	
	unsigned short turnCount = 0; //Counts rounds
	char win = '\0'; //Determines winner, if '\0', then the game hasn't terminated early
	
	while((turnCount < MAXROUNDS) && (win == '\0')){
		
		//Prints the field every 10 turns
		if( turnCount%10 == 0 || VERBOSE){
			printf("Turn: %d\n", turnCount);
			printField(field);
			printPlayers(field);
		}
		
		turnCount++;
		
		win = runRound(field);
		
	}//End while

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
