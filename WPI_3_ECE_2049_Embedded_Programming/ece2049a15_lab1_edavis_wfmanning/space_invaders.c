// Libraries
#include <msp430.h>
#include <stdint.h>
#include "inc\hw_memmap.h"
#include "driverlibHeaders.h"
#include "CTS_Layer.h"
#include <stdlib.h>

#include "grlib.h"
#include "LcdDriver/Dogs102x64_UC1701.h"


// Our includes
 #include "peripherals.h"


#define MAX_INVADERS_COUNT 10
#define LOOPS_PER_MOVE 100
#define COUNTDOWN_DELAY 20000


#define LCD_WIDTH 102
#define LCD_HEIGHT 64 // Approx. value
#define LCD_LINE(y) ((y)*16 + 8)
#define LCD_COLUMN(x) ((x)*(LCD_WIDTH/5) + LCD_WIDTH/10) // There are 5 columns

#define DIGIT_TO_ASCII(x) ((x) + 0x30) // Adding 0x30, the ascii value of 0 to convert a digit to ascii.

#define CHARACTER_HEIGHT 8

// Invaders are approx. 80 (16*5) bits, so the size of this array shouldn't be too ridiculous.
// => Worst comes to worst, we can probably use unsigned chars for everything.
typedef struct invader {
	int x; // 0-4 x location
	int y; // Maybe make a float?
	int id;
	int velocity; // Velocity downwards
	int isAlive; // Should really be a boolean...
} invader;

// Kinda handy, don't know if I'll use it
typedef enum BOOLEAN {
	false = 0,
	true = 1
} BOOLEAN;

typedef enum PROGRAM_STATE {
	start,
	game_over,
	game
} PROGRAM_STATE;

//////////////////////
// Global variables //
//////////////////////

invader invaders[MAX_INVADERS_COUNT];
PROGRAM_STATE state = start;
int current_level = 0;


// Hardware related
void initialization();
CAP_BUTTON readTouchPad();
// readTouchPad();

//Graphics related
void showStartScreen();
void showGameOverScreen();
void countdown();
void drawScreen();
void drawInvader(invader i);

////////////////////////
// Game logic related //
////////////////////////

// Creates initial set of invaders
void gameSetup();

// Runs a single iteration of the game loop - gets input, runs physics, draws, etc...
void runGameLoop();

// Changes worldstate to reflect actions and time passing
void runPhysics(CAP_BUTTON input);

// Populates the field with the next level's invaders
void nextLevel(int level);

invader makeInvader();

// Checks to see if there are any alive invaders
int shouldReplaceInvaders();

// Takes input and "kills off" appropriate invaders
void shootInvaders(CAP_BUTTON input);

// Moves the invaders down towards their goal
void moveInvaders();

// Returns 1 if the game is over, 0 if not
int checkGameOver();



int main() {
	initialization();


	while(1) {
		if(state == start) {
			showStartScreen();
			CAP_BUTTON input = readTouchPad();
			if(input == BIT0) { // When X is pushed
				state = game;
				gameSetup();

				countdown();
			}
		} else if(state == game) {
			runGameLoop();
		} else if(state == game_over) {
			showGameOverScreen();
			state = start;
		}

	}

	return 1; // How did you get here?
}

void initialization() {
	// Stop WDT
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	//Setup for blinking light
	// Set digital IO control registers for  Port 1 Pin 0
	P1SEL = P1SEL & ~BIT0;          // Select P1.0 for digital IO
	P1DIR |= BIT0;			// Set P1.0 to output direction
	__disable_interrupt();          // Not using interrupts so disable them


    //Perform initializations (see peripherals.c)
    configTouchPadLEDs();
    configDisplay();
    configCapButtons();

}

//Reads what TouchPad is pressed, and turns off corresponding LED
//=> Only triggers on button down.
CAP_BUTTON readTouchPad(){
	static CAP_BUTTON last_read;
	CAP_BUTTON input = CapButtonRead();
	// turn off LED of depressed key

	// Turn on all LEDs that don't have the corresponding button pressed.
	P1OUT = (P1OUT | (LED4+LED5+LED6+LED7+LED8)) & ~(input << 1);

	if(last_read == input) {
		return BUTTON_NONE;
	} else {
		last_read = input;
		return input;
	}
}

//////////////////////
// Graphics related //
//////////////////////

void showStartScreen() {
	GrClearDisplay(&g_sContext);
	 GrStringDrawCentered(&g_sContext, "Space Invaders", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(0), TRANSPARENT_TEXT);
	 GrStringDrawCentered(&g_sContext, "Press X", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(2), TRANSPARENT_TEXT);
	 GrStringDrawCentered(&g_sContext, "to start", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(3), TRANSPARENT_TEXT);
	 GrFlush(&g_sContext);
}

void showGameOverScreen() {
	 GrClearDisplay(&g_sContext);
	 GrStringDrawCentered(&g_sContext, "Game Over", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(1), TRANSPARENT_TEXT);

	 GrFlush(&g_sContext);

	 //TODO: Also flash LEDs,  etc...
	 int i, j;
	 for(j=0; j<32; j++) { // 32 = 2^5
		 for(i=0; i < COUNTDOWN_DELAY; i++);
		 P1OUT = P1OUT ^ (j << 1);
	 }
}

void countdown() {
	volatile int i;
	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "3", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(1), TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
	for(i=0; i < COUNTDOWN_DELAY; i++);

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "2", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(1), TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
	for(i=0; i < COUNTDOWN_DELAY; i++);

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "1", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(1), TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
	for(i=0; i < COUNTDOWN_DELAY; i++);
}

void drawScreen() {
	GrClearDisplay(&g_sContext);
	int i;
	for(i=0; i<MAX_INVADERS_COUNT; i++) {
		drawInvader(invaders[i]);
	}
	GrFlush(&g_sContext);
}

void drawInvader(invader inv) {
	if(!inv.isAlive) {
		return;
	}

	char id[2] = "X";
	id[0] = DIGIT_TO_ASCII(inv.id % 10);
	GrStringDraw(&g_sContext, id, 1, LCD_COLUMN(inv.x), inv.y, TRANSPARENT_TEXT);
}

////////////////////////
// Game logic related //
////////////////////////

void gameSetup() {
	current_level = 1;
	int i;
	for(i=0; i<MAX_INVADERS_COUNT; i++) {
		invaders[i].isAlive = false;
	}

	nextLevel(current_level);
}

void runGameLoop() {
	CAP_BUTTON input = readTouchPad();

	runPhysics(input);

	if(checkGameOver()) {
		state = game_over;
	}

	drawScreen();

	// Possibly have a software delay here.
	//   We want it to take < 33 milliseconds to complete a loop (30 fps)
}

void runPhysics(CAP_BUTTON input) {
	if(input) {
		shootInvaders(input);
	}

	moveInvaders();

	if(shouldReplaceInvaders()) {
		nextLevel(current_level++);
	}
}

// Populates the game with the next level's invaders
void nextLevel(int level) {
	char level_string[] = "Level X ";
	level_string[6] = DIGIT_TO_ASCII((level/10) % 10);
	level_string[7] = DIGIT_TO_ASCII(level % 10);

	volatile int i;
	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, level_string, AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(1), TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
	for(i=0; i < COUNTDOWN_DELAY; i++);


	int invaderCount = level * 2 > MAX_INVADERS_COUNT ? MAX_INVADERS_COUNT : level * 2;
	int velocity = level;
	for(i=0; i<invaderCount; i++) {
		invaders[i] = makeInvader();
		invaders[i].velocity = velocity;
	}
}

invader makeInvader() {
	static int id = 0;
	id++;
	invader inv = {
			rand() % 5, // X location
			0, // Y location
			id,
			0, // velocity
			true
	};
	//Prevents collision
	int i;
	for(i = 0; i< MAX_INVADERS_COUNT; i++){
		if(invaders[i].isAlive && invaders[i].y == inv.y){
			inv.y -= CHARACTER_HEIGHT*2;
			i = 0;
		}
	}

	return inv;
}

// Checks to see if there are any alive invaders
int shouldReplaceInvaders() {
	int i;
	for(i=0; i< MAX_INVADERS_COUNT; i++) {
		if(invaders[i].isAlive) {
			return 0;
		}
	}
	return 1;
}

// Takes input and "kills off" appropriate invader
void shootInvaders(CAP_BUTTON input) {
	int column;
	//If multiple buttons are pressed, chooses the leftmost value
	if(BIT0 & input){
		column = 0;
	} else if(BIT1 & input){
		column = 1;
	} else if(BIT2 & input){
		column = 2;
	} else if(BIT3 & input){
		column = 3;
	} else if(BIT4 & input){
		column = 4;
	}
	//Finds the lowest invader in the correct column
	int i, lowest = -1, lowest_invader = -1;
	for(i = 0; i<MAX_INVADERS_COUNT; i++){
		if(invaders[i].isAlive && invaders[i].x == column){
			if(invaders[i].y >lowest){
				lowest = invaders[i].y;
				lowest_invader = i;
			}
		}
	}
	//If an invader exists in that column, kill the invader
	if(lowest_invader != -1){
		invaders[lowest_invader].isAlive = false;
		P1OUT = P1OUT ^ (BIT0);
	}
}

// Moves the invaders down towards their goal
void moveInvaders() {
	// May have a bunch of static variables
	static long int loopCount = 0;
	loopCount++;
	if( ! loopCount % LOOPS_PER_MOVE) {
		return;
	}

	int i;
	for(i=0;i<MAX_INVADERS_COUNT; i++){
		if(invaders[i].isAlive) {
			invaders[i].y += invaders[i].velocity;
		}
	}
}

int checkGameOver() {
	int i;
	for(i = 0; i<MAX_INVADERS_COUNT; i++){
		if(invaders[i].y > (LCD_HEIGHT-CHARACTER_HEIGHT) && invaders[i].isAlive){
			return 1;
		}
	}

	return 0;
}
