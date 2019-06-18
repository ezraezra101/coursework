#include "header.h"

//Ezra Davis
//Homework assignment #2
//CS2303

//Ezra Davis
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

//Ezra Davis
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

//Ezra Davis
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
	for( ; i < TEAMSIZE*2; i++){
		if(field[i].alive && field[i].team != win){
			if(VERBOSE){
				puts("people of both teams are alive!");
			}
			return '\0';
		}
	} //end for
	return win; // Returns '\0' if there are no players left, or the winner
}//End winner

//Ezra Davis
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

//Ezra Davis
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
			field[i*2 + teamsPlaced] = makePlayer(teamName, i+1, rand()%4, row, column);
		}
		else { //Otherwise, try placing the player again
			i--;
		} //end else
	}//end for

} //end placeTeam

//Ezra Davis
//Sets up the field
Player* fieldInitialize(char teamName1, char teamName2){
	if(VERBOSE){
		printf("Initializing the field for a match between %c and %c\n", teamName1, teamName2);
	}
	//An array of Players
	//Keeps track of all the players and their positions
	Player* field = malloc(TEAMSIZE*2 * sizeof(Player));


	int teamFirst = rand()%2; //picks what team goes first. If 0, teamName1 goes first, else teamName2
	placeTeam(field, teamName1, teamFirst);
	placeTeam(field, teamName2, abs(teamFirst-1));
	
	return field;
	
} //end fieldInitialize

