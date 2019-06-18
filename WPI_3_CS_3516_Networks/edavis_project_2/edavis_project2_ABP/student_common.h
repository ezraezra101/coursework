#include <stdio.h>
#include <stdlib.h>

#include "project2.h"

// Creates (and returns) a checksum for the given packet.
int generateChecksum(struct pkt packet);


// Converts a packet to a message. If packet is corrupted, returns 1
int packetToMessage(struct pkt packet, struct msg *message);

// Converts a message to a packet. Always returns 0 because this process never fails
int messageToPacket(struct msg message, struct pkt *packet, int seqnum, int acknum);


///////////////////
// Message Queue //
///////////////////

struct queue_node {
	struct msg data;
	struct queue_node *next;
};
struct queue {
	struct queue_node *first;
	struct queue_node *last;
};

// is the queue empty?
int queueEmpty(struct queue *the_queue);

// Add an item to the front of the queue
void addToQueue(struct queue *the_queue, struct msg data);


struct msg peekFromQueue(struct queue *the_queue);

// Remove the front of the queue and place it in data. If queue is empty, return 1.
int removeFromQueue(struct queue *the_queue);

