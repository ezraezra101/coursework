#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//Ezra Davis
//Homework assignment #2
//CS2303

//Height (rows) and width (columns) of the smashball court. - Both of these should be higher than 1
	//High values of columns may cause the program not to print to screen very well
#define HEIGHT 25
#define WIDTH 25
//The longest a smashball game can go on
#define MAXROUNDS 100
//Size of each smashball team - should be less than (or equal to) HEIGHT * WIDTH / 2
#define TEAMSIZE 9

//Team names: (should be printable ascii characters. Especially not '\0')
#define TEAM1 'A'
#define TEAM2 'B'

//The number of turns between printouts
#define TURNSBETWEENPRINTS 10

//Determines if the turn order is randomized (once, not each round) or alternates
#define RANDOMIZEDTURNORDER false

//Prints debug information when true
#define VERBOSE false

//The direction a player is facing
enum direction {North, East, South, West};

//Each one of these structs represents a player
typedef struct
{
	char team;
	unsigned short number;
	enum direction direction;
	bool alive;//Starts true, once smashed becomes false
	
	unsigned short row;
	unsigned short column;
} Player;

/////////////////////////
//Function prototypes: //
/////////////////////////
	//In model.c:

//Creates a player struct
Player makePlayer(char team, int number, int direction, unsigned short row, unsigned short column);

//Counts the surviving members of the team
unsigned short countTeammembers(Player* field, char teamName);

// Returns teamName if a team has won, otherwise returns '\0'
char winner(Player* field);

//Checks to see if there is a player at the specified row and column
bool checkCollision(Player* field, unsigned short row, unsigned short column);

//Places one of the two teams onto the field
void placeTeam(Player* field, char teamName, unsigned short teamsPlaced);

//Sets North the field
Player* fieldInitialize(char teamName1, char teamName2);


	//In controller.c:

//returns true if the player is on another team, and kills the player hopped onto, otherwise returns false
bool hopOnto(Player* field, char hopperTeam, unsigned short row, unsigned short column);

//Places a player and determines consequences of new placement
void placePlayer(Player* field, Player* activePlayer, unsigned short rowTo, unsigned short columnTo);

//Moves a single player
void movePlayer(Player* field, Player* activePlayer);

//Randomizes turn order
void randomizeTurnOrder(Player* field);

//Runs a single round of SmashBall. Returns teamName if a team has won. Otherwise, returns '\0'
char runRound(Player* field);

//plays a game of smashball
int main();


	//In view.c:

//Returns the direction of the player as a string
char *getDirection(Player player);

//prints players names & location
void printPlayers(Player* field);

//Prints the player icon at the row and column, or "  "
void printPlayerIcon(Player* field, unsigned short row, unsigned short column);

//Prints the smashball field
void printField(Player* field);


