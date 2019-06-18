#include "student_common.h"

// Contains functions related to the Receiver (called endpoint B) of the Alternating bit protocol.


// The sequence number that the next packet expects
int expected_packet_seqnum = 0;

// Returns the appropriate acknoledgement for the current state
struct pkt current_acknowledgement() {
	const static struct msg arbitrary = {
		"1234567890123456789"
	};
	
	struct pkt packet;
	messageToPacket(arbitrary, &packet, 0, expected_packet_seqnum); // 0->doesn't contain 'data'
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
	
	if(!packetToMessage(packet, &message) && expected_packet_seqnum == packet.seqnum) {
		expected_packet_seqnum = expected_packet_seqnum ? 0 : 1;
		tolayer5(BEntity, message);
	}
	
	tolayer3(BEntity, current_acknowledgement());
}


/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
	// We aren't using this function!
}

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
}

