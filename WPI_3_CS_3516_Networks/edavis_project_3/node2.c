#include <stdio.h>
#include "common.h"

#define NODE_NUMBER 2

struct distance_table dt2;

/* students to write the following two routines, and maybe some others */

void rtinit2() {
   dt2 = init_dt(NODE_NUMBER);
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
    dt2 = update(NODE_NUMBER, dt2, rcvdpkt);
    if(TraceLevel >= 1) printdt(NODE_NUMBER, &dt2);
}
