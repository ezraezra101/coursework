#define MAX_INVADERS_COUNT 5


// Invaders are approx. 80 (16*5) bits, so the size of this array shouldn't be too ridiculous.
// => Worst comes to worst, we can probably use unsigned chars for everything.
typedef struct invader {
	int x; // 0-4 x location
	int y; // Maybe make a float?
	int id;
	int velocity; // Velocity downwards
	int isDead; // Should really be a boolean...
} invader;

// Kinda handy
typedef enum BOOLEAN {
	false = 0,
	true = 1
} BOOLEAN;

typedef enum PROGRAM_STATE {
	start,
	game_over,
	game
} PROGRAM_STATE;


invader invaders[MAX_INVADERS_COUNT];


void initialization();
void showStartScreen();
void showGameOverScreen();
int getCapInput();
void shootInvaders(int input);	// Iterates through invaders[], and makes the appropriate one dead.
void moveInvaders();			// May have a bunch of static variables
int checkGameOver();

int main() {
	initialization();


	while(1) {
		if(state == in_start_screen) {
			showStartScreen();
			input = getCapInput();
			if(input) {
				state = game;

				gameSetup();
			}
		} else if(state == game) {
			run_game_loop();
		} else if(state == game_over) {
			showGameOverScreen();
			if(getCapInput()) {
				state = start;
			}
		}

	}

	return 1; // How did you get here?
}

gameSetup() {
	for(int i=0; i<INVADERS_COUNT; i++) {
		invaders[i] = makeInvader(); 
	}
}

invader makeInvader() {
	// Do stuff...
	// Prevent collsion
}


void run_game_loop() {
	input = getCapInput();

	isGameOver = runPhysics(input); // Bad function name!

	if(isGameOver) {
		state = game_over;
	}

	clearScreen();
	drawScreen();

	// Possibly have a software delay here.
	//   We want it to take < 33 milliseconds to complete a loop (30 fps)
}


int runPhysics(int input) {
	if(input) {
		shootInvaders(input);
	}

	moveInvaders();

	if(shouldReplaceInvaders()) {
		for(int i=0; i<INVADERS_COUNT; i++) {
			invaders[i] = makeInvader();
		}
	}

	return checkGameOver();
}

void drawScreen() {
	for(int i=0; i<INVADERS_COUNT; i++) {
		drawInvader(invaders[i]);
	}
}