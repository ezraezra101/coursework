#include "student_common.h"

// Contains functions related to the Receiver (called endpoint B) of the Go-Back-N protocol


///////////////////////
// Window management //
///////////////////////

int window_min;
struct msg window[WINDOW_SIZE];
char window_status[WINDOW_SIZE];

// Print out debug information:
void printWindowState() {
	int i;
	printf("Receiver window state:\n");
	for(i=0; i<WINDOW_SIZE; i++) {
		if(window_status[i] == TRUE)
			printf("\t%d", window_min + i);
		else
			printf("\tX");
	}
	printf("\n");
}

// Lowest packet number in the window
int getWindowMin() {
	return window_min;
}

// Updates window & sends packets to layer 5 as appropriate
void addToWindow(int sequence_number, struct msg message) {
	if( (sequence_number - window_min) < WINDOW_SIZE && sequence_number >= window_min) {
#ifdef DEBUG
		printf("Received packet %d\n", sequence_number);
#endif
		window[sequence_number-window_min] = message;
		window_status[sequence_number-window_min] = TRUE;
	}

	// If this is the next expected packet, send as many packets to layer 5 as you can.
	while(window_status[0] == TRUE) {
		tolayer5(BEntity, window[0]);
		int i;
		for(i=0; i<WINDOW_SIZE-1; i++) {
			window[i] = window[i+1];
			window_status[i] = window_status[i+1];
		}
		window_status[WINDOW_SIZE-1] = FALSE;
		window_min++;
	}

	return;
}

struct pkt current_acknowledgement() {
	const static struct msg arbitrary = {
		"1234567890123456789"
	};
	
	struct pkt packet;
	messageToPacket(arbitrary, &packet, 0, getWindowMin()); // 0->doesn't contain 'data'
	return packet;
}


/*
 * Just like A_output, but residing on the B side.  USED only when the 
 * implementation is bi-directional.
 */
void B_output(struct msg message)  {
	// We aren't using this function!
}

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {
	struct msg message;

	if(!packetToMessage(packet, &message)) { // If packet is valid
		addToWindow(packet.seqnum, message);
	}
#ifdef DEBUG
	printWindowState();
#endif
	tolayer3(BEntity, current_acknowledgement());
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
}

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
	window_min = 0;
	int i;
	for(i=0; i< WINDOW_SIZE; i++) {
		window_status[i] = FALSE;
	}
}


