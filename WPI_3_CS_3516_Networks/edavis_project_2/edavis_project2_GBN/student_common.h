#include <stdio.h>
#include <stdlib.h>

#include "project2.h"

#define WINDOW_SIZE 8

// #define DEBUG

// Creates (and returns) a checksum for the given packet.
int generateChecksum(struct pkt packet);


// Converts a packet to a message. If packet is corrupted, returns 1
int packetToMessage(struct pkt packet, struct msg *message);

// Converts a message to a packet. Always returns 0 because this process never fails
int messageToPacket(struct msg message, struct pkt *packet, int seqnum, int acknum);
