#include "student_common.h"

// Creates (and returns) a checksum for the given packet.
int generateChecksum(struct pkt packet) {
	packet.checksum = 0;
	int i;
	int checksum = 0;
	// My extraordinarily lazy checksum generator.
	for(i=0; i< sizeof(struct pkt); i++) {
		unsigned char *x = i + (unsigned char *) &packet;
		checksum += i * (*x);
	}
	return checksum;
}

// Converts a packet to a message. If packet is corrupted, returns 1
int packetToMessage(struct pkt packet, struct msg *message) {
	if(generateChecksum(packet) != packet.checksum) {
		return 1;
	} else {
		int i;
		for(i=0; i< MESSAGE_LENGTH; i++) {
			message->data[i] = packet.payload[i];
		}
		return 0;
	}
}

// Converts a message to a packet. Always returns 0 because this process never fails
int messageToPacket(struct msg message, struct pkt *packet, int seqnum, int acknum) {
	
	int i;
	for(i=0; i< MESSAGE_LENGTH; i++) {
		packet->payload[i] = message.data[i];
	}
	
	packet->seqnum = seqnum;
	packet->acknum = acknum;
	
	packet->checksum = generateChecksum(*packet);
	
	return 0;
}
