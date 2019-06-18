#include <stdio.h>
#include "common.h"

#define NODE_NUMBER 0

struct distance_table dt0;

/* students to write the following two routines, and maybe some others */

void rtinit0() {
    dt0 = init_dt(NODE_NUMBER);
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    dt0 = update(NODE_NUMBER, dt0, rcvdpkt);
    if(TraceLevel >= 1) printdt(NODE_NUMBER, &dt0);
}

