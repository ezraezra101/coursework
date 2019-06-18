#include "common.h"
#include "stdlib.h" // For free
#include "stdio.h"

// Informs neighbors of distance table changes
void inform_neighbors(int node_num, struct distance_table dt) {
	struct NeighborCosts *neighbors = getNeighborCosts(node_num); // Way of figuring out connections
	struct RoutePacket packet;
	int i, j;

   	// Construct packet
    packet.sourceid = node_num;
    for(i=0; i<MAX_NODES; i++) {
    	packet.mincost[i] = dt.costs[node_num][i];
    }

    // Figure out nodes to poison.
    int next[MAX_NODES] = {0,0,0,0}; // next[0] is the next node to travel to to reach node 0.
    int temp_costs[MAX_NODES] = {INFINITY, INFINITY, INFINITY, INFINITY};
    for(i=0; i<MAX_NODES; i++) {
        for(j=0; j<MAX_NODES; j++) {
            // Note: neigbors->NodeCosts[j] may be INFINITY.
            if(temp_costs[i] > dt.costs[j][i] + neighbors->NodeCosts[j]) {
                next[i] = j;
                temp_costs[i] = dt.costs[j][i] + neighbors->NodeCosts[j];
            }
        }
    }

    // Ship off information
    for(i=0; i<MAX_NODES; i++) {
    	// If it's a neighbor
    	if(i != node_num && neighbors->NodeCosts[i] < INFINITY) {
    		packet.destid = i;

            if(TraceLevel >= 0) {
                printf("At time t=%f, node %d sends packet to node %d with: %d %d %d %d\n",
                    clocktime, packet.sourceid, packet.destid,
                    packet.mincost[0], packet.mincost[1], packet.mincost[2], packet.mincost[3]);
            }

            // Poison reverse
            struct RoutePacket to_send_packet = packet;
            for(j=0; j<MAX_NODES; j++) {

                if(next[j] == i) {
                    to_send_packet.mincost[j] = INFINITY;
                }
            }

    		toLayer2(to_send_packet);
    	}
    }

    free(neighbors);
}

// Creates a distance table for the given node, and sends out appropriate packets
struct distance_table init_dt(int node_num) {
	if(TraceLevel >= 0) printf("At time t=%f, rtinit%d() called.\n", clocktime, node_num);


    struct NeighborCosts *neighbors;
    struct distance_table dt;
    int i, j;

    // Initialize dt to infinities
    for(i=0; i<MAX_NODES; i++) {
    	for(j=0; j<MAX_NODES; j++) {
    		dt.costs[i][j] = i == j ? 0 : INFINITY; // Distance to self is 0.
    	}
    }

    // Save neighbor distances into dt
    neighbors = getNeighborCosts(node_num);
    for(j=0; j<neighbors->NodesInNetwork; j++) {
    	dt.costs[node_num][j] = neighbors->NodeCosts[j];
    }

    free(neighbors);

    inform_neighbors(node_num, dt);

    return dt;
}


struct distance_table update(int node_num, struct distance_table dt, struct RoutePacket *rcvdpkt) {
	if(TraceLevel >= 0) printf("At time t=%f, rtupdate%d() called.\n", clocktime, node_num);


	int i;
	char changed_dt = NO;
	// Copy updated value into distance table
	for(i=0; i<MAX_NODES; i++) {
        if(TraceLevel >= 2 &&
            dt.costs[rcvdpkt->sourceid][i] != rcvdpkt->mincost[i]) {
            // Only print changes.
            printf("Replacing %d's tables distance from %d to %d with cost %d\n",
                node_num, rcvdpkt->sourceid,i, rcvdpkt->mincost[i]);
        }

		dt.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
	}

	// Check if other numbers decrease
	for(i=0; i<MAX_NODES; i++) {
		int dist_through_new = dt.costs[rcvdpkt->sourceid][i] + dt.costs[node_num][rcvdpkt->sourceid];
		if(dist_through_new < dt.costs[node_num][i]) {
			dt.costs[node_num][i] = dist_through_new;
			changed_dt = YES;
		}
	}

	if(changed_dt) {
		inform_neighbors(node_num, dt);
	}

	return dt;
}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt( int MyNodeNumber, struct distance_table *dtptr ) {
	struct NeighborCosts *neighbor = getNeighborCosts(MyNodeNumber);
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (TraceLevel >= 2 || (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber ))  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( TraceLevel >= 2 || i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[Neighbors[j]][i] );
            }
            printf("\n");
        }
    }
    printf("\n");

    free(neighbor);
}    // End of printdt

