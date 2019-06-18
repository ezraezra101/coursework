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


///////////////////
// Message Queue //
///////////////////


// is the queue empty?
int queueEmpty(struct queue *the_queue) {
	return the_queue->first == NULL;
}

// Exits and prints if queue doesn't have basic integrity (otherwise returns 0)
int queueIntegrity(struct queue *the_queue) {
	if((the_queue->first == NULL) != (the_queue->last == NULL)) {
		fprintf(stderr, "The queue isn't sure if it's empty\n");
		exit(1);
	}
	if(the_queue->last && the_queue->last->next != NULL) {
		fprintf(stderr, "The queue doesn't know when it ends\n");
		exit(1);
	}
	struct queue_node *node = the_queue->first;
	while(node != NULL && node->next != NULL) {
		node = node->next;
	}
	if( (node != NULL) && (node != the_queue->last)) {
		fprintf(stderr, "The queue doesn't go from first to last!\n");
		exit(1);
	}
	
	return 0;
}

// Add an item to the front of the queue
void addToQueue(struct queue *the_queue, struct msg data) {
	queueIntegrity(the_queue);
	
	struct queue_node *node = malloc(sizeof(struct queue_node));
	node->data = data;
	node->next = NULL;
	if(queueEmpty(the_queue)) {
		// The queue is empty
		the_queue->first = node;
		the_queue->last = node;
	} else {
		the_queue->last->next = node;
		the_queue->last = node;
	}
}

struct msg peekFromQueue(struct queue *the_queue) {
	queueIntegrity(the_queue);
	
	if(queueEmpty(the_queue)) {
		struct msg blank;
		return blank;
	} else {
		return the_queue->first->data;
	}
}

// Remove the front of the queue and place it in data. If queue is empty, return 1.
int removeFromQueue(struct queue *the_queue) {
	queueIntegrity(the_queue);
	if(queueEmpty(the_queue)) {
		//*data = NULL;
		return 1;
	} else {
		struct queue_node *node = the_queue->first;
		if(the_queue->first == the_queue->last) {
			// if we're emptying the queue
			the_queue->last = NULL;
		} else {
			the_queue->first = node->next;
		}
		free(node);
		return 0;
	}
}
