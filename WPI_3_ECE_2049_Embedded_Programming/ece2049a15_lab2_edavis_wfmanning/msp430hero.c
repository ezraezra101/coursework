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


#define NOTE_DURATION   100  // 100 * 0.005 sec = 0.5s
#define COUNTDOWN_DELAY 200 // 1 second
#define SCREEN_REFRESH  60 // 60 * 0.005 = 0.3 sec

#define LCD_WIDTH 102
#define LCD_HEIGHT 64
#define LCD_LINE(y) ((y)*16 + 8)
#define LCD_COLUMN(x) ((x)*(LCD_WIDTH/5) + LCD_WIDTH/10) // There are 5 columns

#define DIGIT_TO_ASCII(x) ((x) + 0x30) // Adding 0x30, the ascii value of 0 to convert a digit to ascii.

#define CHARACTER_HEIGHT 8


#define S1_PUSHED() (!(P1IN & BIT7)) // Pushbutton 1 is pushed
#define S2_PUSHED() (!(P2IN & BIT2)) // Pushbutton 2 is pushed

typedef enum PROGRAM_STATE {
	start,
	start_countdown,
	game_over,
	game
} PROGRAM_STATE;

typedef enum PITCH {
	pA=440,
	pBflat=466,
	pB=494,
	pC=523,
	pCsharp=554,
	pD=587
} PITCH;

typedef enum BOOL {
	true=1,
	false=0
} BOOL;

//////////////////////
// Global variables //
//////////////////////

#define SONG_LENGTH 17
enum PITCH song[] = {pCsharp, pB, pA, pCsharp, pB, pA, pA, pA, pA, pA, pB, pB, pB, pB, pCsharp, pB, pA };
long int timer, leap_cnt;
int score;
long int last_drew_to_screen;

#define NOTES_ON_SCREEN 8

void initialization();

void startNote(int);
void stopNote();

void showStartScreen();
void showGameOverScreen();
int countdown();
void drawScreen();
int noteColumn(PITCH pitch); // Returns the correct column for a given note
void displayNote(PITCH note, int y); // Displays the given pitch at the height y.

int getInputColumn(); // Returns 0-4 (or -1), the column currently being input.

// Runs a single iteration of the game loop - gets input, runs physics, draws, etc...
int runGameLoop();


int main() {
	initialization();

	enum PROGRAM_STATE state = start;

	showStartScreen();

	while(1) {

		// Reset button!
		if(S2_PUSHED()) {
			state = start;
			stopNote();
			// Turn off the LEDs
			P8OUT &= ~(BIT0 | BIT2);
			P1OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

			showStartScreen();
		}


		switch(state) {
		case start:
			if(S1_PUSHED()) {
				state = start_countdown;
				timer = 0;
			}
			break;
		case start_countdown:
			if(countdown()) {
				state = game;
				timer = -NOTE_DURATION;
				score = 0;
				last_drew_to_screen = -SCREEN_REFRESH;
			}
			break;
		case game:
			if(runGameLoop()) {
				state = game_over;
				showGameOverScreen();
				timer = 0;
				// Turn off the LEDs
				P8OUT &= ~(BIT0 | BIT2);
				P1OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
			}
			break;
		case game_over:
			 if(timer > COUNTDOWN_DELAY) {
				timer = 0;
				P1OUT = P1OUT ^ (BIT1|BIT2|BIT3|BIT4|BIT5); // Flash LEDs
			 }
			break;
		default:
			break;
		}
	}


	return 1; // How did you get here?
}

void initialization() {
	// Stop WDT
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	// Setup Buttons
	P1SEL &= ~BIT7; // Switch/pushbutton 1
	P1DIR &= ~BIT7;
	P2SEL &= ~BIT2; // Switch/pushbutton 2
	P2DIR &= ~BIT2;

	// Setup LEDs
	P1SEL &= ~BIT0; // LED1
	P1DIR |= BIT0;
	P8SEL &= ~(BIT1|BIT2); //LED2 and LED3
	P8DIR |= BIT1 | BIT2;

    //Perform initializations (see peripherals.c)
    configTouchPadLEDs();
    configDisplay();
    configCapButtons();

    // Setup timer A2 to use ACLK (TASSEL_1), have a divider of 1 (ID_0) and use Up mode (MC_1)
    _BIS_SR(GIE); // Global Interrupt Enable

    TA2CTL = TASSEL_1 + ID_0 + MC_1;
    TA2CCR0 = 163; // 163 + 1= 164 ACLK ticks = ~0.005 seconds
    TA2CCTL0= CCIE; // Enable TA2CCR0 interrupt.
    timer = 0;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void) {
	// 1/32768 seconds * (max_count + 1) = t_int = 0.005
	// max_count + 1 = 163.84
	// Becomes inaccurate after 1025 interrupts.
	if(leap_cnt < 1025) {
		timer++;
		leap_cnt++;
	} else {
		leap_cnt = 0;
	}
}

/*
 * Enable a PWM-controlled buzzer on P7.5
 * This function makes use of TimerB0.
 */
// Plays the frequency (in Hz) given
void startNote(int frequency)
{
	// Initialize PWM output on P7.5, which corresponds to TB0.3
	P7SEL |= BIT5; // Select peripheral output mode for P7.5
	P7DIR |= BIT5;

	TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
	TB0CTL  &= ~TBIE; 						// Explicitly Disable timer interrupts for safety

	// Now configure the timer period, which controls the PWM period
	// Doing this with a hard coded values is NOT the best method
	// I do it here only as an example. You will fix this in Lab 2.
	TB0CCR0   = 32768/frequency; 					// Set the PWM period in ACLK ticks
	//32728hz = ACLK ticks per second.

	TB0CCTL0 &= ~CCIE;					// Disable timer interrupts

	// Configure CC register 3, which is connected to our PWM pin TB0.3
	TB0CCTL3  = OUTMOD_7;					// Set/reset mode for PWM
	TB0CCTL3 &= ~CCIE;						// Disable capture/compare interrupts
	TB0CCR3   = TB0CCR0/2; 					// Configure a 50% duty cycle
}

/*
 * Disable the buzzer on P7.5
 */
void stopNote(void) {
	// Disable both capture/compare periods
	TB0CCTL0 = 0;
	TB0CCTL3 = 0;
}

//////////////////////
// Graphics related //
//////////////////////

void showStartScreen() {
	GrClearDisplay(&g_sContext);
	 GrStringDrawCentered(&g_sContext, "MSP430 Hero", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(0), TRANSPARENT_TEXT);
	 GrStringDrawCentered(&g_sContext, "Press Button 1", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(2), TRANSPARENT_TEXT);
	 GrStringDrawCentered(&g_sContext, "to start", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(3), TRANSPARENT_TEXT);
	 GrFlush(&g_sContext);
}

void showGameOverScreen() {
	char your_score[] = "Your score is XX";
	your_score[14] = DIGIT_TO_ASCII(score/10);
	your_score[15] = DIGIT_TO_ASCII(score%10);

	char *win_lose = score > (3*SONG_LENGTH/4) ? "You win!": "You lose :-(";

	GrClearDisplay(&g_sContext);
	GrStringDrawCentered(&g_sContext, "Game Over", AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(1), TRANSPARENT_TEXT);
	GrStringDrawCentered(&g_sContext, your_score, AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(2), TRANSPARENT_TEXT);
	GrStringDrawCentered(&g_sContext, win_lose, AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(3), TRANSPARENT_TEXT);
	GrFlush(&g_sContext);
}

int countdown(void) {
	 static int count = 3; // count is: The current number displayed (3 to 0. -1 indicates moving to next state.)

	static long int last_timer = -COUNTDOWN_DELAY; // Keeps track of time spent at a given count.

	// If we've waited for long enough.
	if(timer-last_timer >= COUNTDOWN_DELAY) {
		last_timer = timer;

		// Figure out text to display
		char *countdown = NULL;
		switch (count) {
		case 3:
			P8OUT |= BIT2; // Turn on LED3
			countdown = "3";
			break;
		case 2:
			P8OUT = (P8OUT & ~BIT2) | BIT1; // Turn on LED2, turn off LED3
			countdown = "2";
			break;
		case 1:
			P8OUT &= ~BIT1; // Turn off LED2
			P1OUT |= BIT0; // Turn on LED1
			countdown = "1";
			break;
		case 0:
			P8OUT |= BIT2 | BIT1; // Turn the remaining LEDs on
			countdown = "GO";
			break;
		default: // Case -1
			P8OUT &= ~(BIT1 | BIT2);// Turn off the LEDs
			P1OUT &= ~BIT0;
			count = 3;
			last_timer = 0;
			return 1; // Move to next program flow state.
		}

		count--;

		// And display it!
		GrClearDisplay(&g_sContext);
		GrStringDrawCentered(&g_sContext, countdown, AUTO_STRING_LENGTH, LCD_WIDTH/2, LCD_LINE(1), TRANSPARENT_TEXT);
		GrFlush(&g_sContext);
	}

	return 0;
}

void drawScreen() {

	if(timer - last_drew_to_screen > SCREEN_REFRESH	// If some time has passed since we last drew
			|| last_drew_to_screen > timer) {		// Or we've restarted
		last_drew_to_screen = timer;

		GrClearDisplay(&g_sContext);
		int i;
		for(i=0; i<SONG_LENGTH; i++) {

			// Calculating note Y position:

			int initial_y = LCD_HEIGHT-CHARACTER_HEIGHT - i*NOTE_DURATION/5;
			int note_y = initial_y + timer/5;

			if(note_y >= 0 && note_y < LCD_HEIGHT - CHARACTER_HEIGHT)
				displayNote(song[i], note_y);
		}
		GrFlush(&g_sContext);
	}
}

// Returns the correct column for a given note
int noteColumn(PITCH pitch) {
	switch (pitch) {
	case pA:
		return 0;
	case pBflat:
	case pB:
		return 1;
	case pC:
		return 2;
	case pCsharp:
		return 3;
	case pD:
		return 4;
	default:
		return 4; // Why not make 4 default?
	}
}

// Displays the given pitch at the height y.
void displayNote(PITCH note, int y) {
	char note_char[3] = "X";

	switch (note) {
	case pA:
		note_char[0] = 'A';
		break;
	case pBflat:
		note_char[0] = 'B';
		note_char[1] = 'b';
		note_char[2] = '\0';
		break;
	case pB:
		note_char[0] = 'B';
		break;
	case pC:
		note_char[0] = 'C';
		break;
	case pCsharp:
		note_char[0] = 'C';
		note_char[1] = '#';
		note_char[2] = '\0';
		break;
	case pD:
		note_char[0] = 'D';
		break;
	default:
		break;
	}

	int x = LCD_COLUMN(noteColumn(note));

	GrStringDraw(&g_sContext, note_char, AUTO_STRING_LENGTH, x, y, TRANSPARENT_TEXT);
}

// Gets the column of the currently pressed button.
int getInputColumn() {
	CAP_BUTTON input = CapButtonRead();
	int column = -1;

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
	return column;
}

int runGameLoop(int clock_ticks_since_game_start) {

	static char note_index = 0;

	static BOOL note_pushed = true;

	// Start next note if loop count has exceeded the duration
	if(timer > NOTE_DURATION * note_index) {
		stopNote();
		if(note_index < SONG_LENGTH) {
			startNote(song[note_index]);

			// Turn off touchpad LEDs, except for LED corresponding to song's current note.
			P1OUT = (P1OUT & ~(LED4+LED5+LED6+LED7+LED8)) | (2 << noteColumn(song[note_index]));
			// Turn off LED 3:
			P8OUT &= ~BIT2;

			// Turn LED1 on iff you failed pushing the last note.
			P1OUT = note_pushed ? P1OUT & ~BIT0 : P1OUT | BIT0;

			note_pushed = false;
		}
		else {
			note_index = 0;
			note_pushed = true;
			return 1; // Song is over!
		}

		note_index++;
	}

	if(getInputColumn() == noteColumn(song[note_index-1]) && !note_pushed) {
		note_pushed = true;
		// Turn off the LED corresponding to the button pushed
		P1OUT &= ~(2 << noteColumn(song[note_index]));

		// Turn on the green LED (LED3)
		P8OUT |= BIT2;

		score++;
	}

	drawScreen();

	return 0;
}
