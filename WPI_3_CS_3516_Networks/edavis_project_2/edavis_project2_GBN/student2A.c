#include "student_common.h"

#define BUFFER_SIZE 512

// A's state:


//// Timeout time calculations
#define EXPECTED_RTT_WEIGHT 0.125 // Weight of the sample_rtt when updating the expected_rtt
#define DEVIATION_WEIGHT 0.25 // Weight of the deviation from the new sample_rtt when calculating the running average

double expected_rtt;
double rtt_deviation;

char no_timeouts_occured;
double last_timer_reset;

double current_timeout_length() {
	return expected_rtt + 4 * rtt_deviation;
}
void calculate_timeout_length(double sample_rtt) {
	double deviation = expected_rtt - sample_rtt;

	expected_rtt = (1-EXPECTED_RTT_WEIGHT) * expected_rtt + sample_rtt * EXPECTED_RTT_WEIGHT;

	rtt_deviation = (1-EXPECTED_RTT_WEIGHT) * rtt_deviation + EXPECTED_RTT_WEIGHT * (deviation < 0 ? -deviation : deviation);
}




int lowest_unacked_seqnum; // The sequence number of messages[0].

struct msg messages[BUFFER_SIZE];
int messages_in_buffer; // The total number of messages waiting to be sent/ACKed.
int messages_in_transit; // The number of already sent messages still in the buffer.

// Add a message to the message buffer.
void add_message_to_buffer(struct msg message) {
	if(messages_in_buffer+1 > BUFFER_SIZE) {
		printf("Buffer overflow! - only %d packets can be buffered before sending.\n", BUFFER_SIZE);
		exit(1);
	}
	messages[messages_in_buffer] = message;
	messages_in_buffer++;
}

// Sends messages in the buffer that are queued up to send, with a limit of having WINDOW_SIZE unacked messages
void send_possible_messages() {
	while(messages_in_transit < WINDOW_SIZE && messages_in_transit < messages_in_buffer) {
#ifdef DEBUG
		printf("Sending packet %d\n", lowest_unacked_seqnum+messages_in_transit);
#endif
		struct pkt packet;

		messageToPacket(messages[messages_in_transit], &packet, lowest_unacked_seqnum + messages_in_transit, 0);

		tolayer3(AEntity, packet);
		messages_in_transit++;
	}

	if(!getTimerStatus(AEntity) && messages_in_transit) {
		startTimer(AEntity, current_timeout_length());
		last_timer_reset = getClockTime();
	} else if(!messages_in_transit) {
		stopTimer(AEntity);
	}
}

// Assume all unACKed messages are lost, and resend them.
void resend_messages() {
	messages_in_transit = 0;
	send_possible_messages();
}

// Move window so that the minimum is seqnum
// Returns TRUE if the window moved
int move_window(int seqnum) {
#ifdef DEBUG
	printf("%d has been acked!\n", seqnum-1);
#endif
	if(seqnum < lowest_unacked_seqnum) {
		// Don't decrease the sequence number.
		return FALSE;
	} else {
		int i, packets_to_discard = seqnum - lowest_unacked_seqnum;
		lowest_unacked_seqnum = seqnum;
		messages_in_buffer += -packets_to_discard;
		messages_in_transit+= -packets_to_discard;

		for(i = 0; i < BUFFER_SIZE - packets_to_discard; i++) {
			messages[i] = messages[i+packets_to_discard];
		}

		return TRUE;
	}
}



/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
/* 
 * The routines you will write are detailed below. As noted above, 
 * such procedures in real-life would be part of the operating system, 
 * and would be called by other procedures in the operating system.  
 * All these routines are in layer 4.
 */

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
void A_output(struct msg message) {
	add_message_to_buffer(message);

	send_possible_messages();
}

/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {

	struct msg message;
	if(!packetToMessage(packet, &message)) {
		if(move_window(packet.acknum)) {
			// Window moved!
			calculate_timeout_length(getClockTime() - last_timer_reset);
			startTimer(AEntity, current_timeout_length());
			last_timer_reset = getClockTime();
		}
	}

	send_possible_messages();

}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
#ifdef DEBUG
	printf("\n\n\nTIMEOUT! (%lf since clock start)\n\n\n\n", current_timeout_length());
#endif
	resend_messages();
}

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
	expected_rtt = 10.0; // Starting expected round trip time. - a somewhat arbitrary number
	rtt_deviation = 0.0; // Starting eviation from the expected rtt.

	messages_in_buffer = 0;
	messages_in_transit = 0;


}


