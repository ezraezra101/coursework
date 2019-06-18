#ifndef COMMON_H
#define COMMON_H

#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
// distance_table[0][1] represents the distance 1 is from 0.
// Which wasn't the assumption implied by the assignment!


// Creates a distance table for the given node, and sends out appropriate packets
struct distance_table init_dt(int node_num);

// Modifies a distance table for the givent node, and sends out appropriate packets
struct distance_table update(int node_num, struct distance_table dt, struct RoutePacket *rcvdpkt);

// Prints a distance table.
// Modified version of the printdt0 from the assignment.
//  Calls getNeighborCosts() instead of receiving an argument.
// 	Also matches modified distance_table...
void printdt( int MyNodeNumber, struct distance_table *dtptr );

#endif
