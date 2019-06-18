#include "student_common.h"

// This file contains functions & state information about the Sender of the Alternating Bit Protocol

/* ***************************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for Project 2, unidirectional or bidirectional
   data transfer protocols from A to B and B to A.
   Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets may be delivered out of order.

   Compile as gcc -g project2.c student2.c -o p2
**********************************************************************/

// A's state:

// The sequence number of the packet that we've most recently sent.
int current_packet_seqnum;

// The queue of messages waiting to be sent.
// => The first item in the queue may have been sent and be waiting for an ACK.
struct queue message_queue;

// Gets the packet from the front of the queue
struct pkt current_packet() {
	struct pkt packet;
	messageToPacket(peekFromQueue(&message_queue), &packet, current_packet_seqnum, 0); // 0-> not an ack.
	return packet;
}

// Returns whether the queue has packets
int packets_in_queue() {
	return !queueEmpty(&message_queue);
}
// Move to next message in the queue, discarding the current one.
void move_to_next_packet() {
	if(packets_in_queue()) {
		removeFromQueue(&message_queue);
	}
	current_packet_seqnum = current_packet_seqnum ? 0 : 1;
}
// Add a message to the message queue.
void add_message_to_queue(struct msg message) {
	addToQueue(&message_queue, message);
}


//// Timeout time calculations
#define EXPECTED_RTT_WEIGHT 0.125 // Weight of the sample_rtt when updating the expected_rtt
#define DEVIATION_WEIGHT 0.25 // Weight of the deviation from the new sample_rtt when calculating the running average

double expected_rtt;
double rtt_deviation;

char no_timeouts_occured;
double last_packet_leaving_time;

double current_timeout_length() {

	//printf("Current timer time: %lf\n", expected_rtt + 4 * rtt_deviation);
	return expected_rtt + 4 * rtt_deviation;
}
void calculate_timeout_length(double sample_rtt) {
	double deviation = expected_rtt - sample_rtt;

	expected_rtt = (1-EXPECTED_RTT_WEIGHT) * expected_rtt + sample_rtt * EXPECTED_RTT_WEIGHT;

	rtt_deviation = (1-EXPECTED_RTT_WEIGHT) * rtt_deviation + EXPECTED_RTT_WEIGHT * (deviation < 0 ? -deviation : deviation);
}




/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
void A_output(struct msg message) {
	int busy = packets_in_queue();
	add_message_to_queue(message);
	
	if(!busy) {
		// We don't already have a packet traveling to B
		tolayer3( AEntity, current_packet() );
		last_packet_leaving_time = getClockTime();
		no_timeouts_occured = TRUE;
		startTimer(AEntity, current_timeout_length());
	} else {
		// Wait until the acknoledgment(s) come back.
	}
}

/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
	struct msg message;
	if(!packetToMessage(packet, &message)
		&& packet.acknum != current_packet_seqnum) {
		// B expects the packet with the next acknum/seqnum
		move_to_next_packet();
		if(no_timeouts_occured) {
			calculate_timeout_length(getClockTime() - last_packet_leaving_time);
		}
	}
	if(packets_in_queue()) {
		tolayer3(AEntity, current_packet());
		last_packet_leaving_time = getClockTime();
		no_timeouts_occured = TRUE;
		startTimer(AEntity, current_timeout_length());
	} else {
		stopTimer(AEntity); // Technically not necessary
	}
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
	// If we're waiting for an ACK
	if(packets_in_queue()) {
		// Resend packet... It's probably gotten lost.
		tolayer3(AEntity, current_packet());
		no_timeouts_occured = FALSE;
		startTimer(AEntity, current_timeout_length());
	} else {
		stopTimer(AEntity);
	}
}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
	current_packet_seqnum = 0;

	message_queue.first = NULL;
	message_queue.last  = NULL;

	expected_rtt = 10.0; // Starting expected round trip time. - a somewhat arbitrary number
	rtt_deviation = 0.0; // Starting eviation from the expected rtt.
	no_timeouts_occured = FALSE;

}
