#include <stdio.h>
#include "common.h"

#define NODE_NUMBER 1

struct distance_table dt1;

/* students to write the following two routines, and maybe some others */

void rtinit1() {
   dt1 = init_dt(NODE_NUMBER);
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
    dt1 = update(NODE_NUMBER, dt1, rcvdpkt);
    if(TraceLevel >= 1) printdt(NODE_NUMBER, &dt1);
}
