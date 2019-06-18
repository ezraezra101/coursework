#include "voronoi_query.cpp"
//#include "test_subdivide_cell.cpp"
#include <stdio.h>
#include <stdlib.h>

FP rho(point q) { return (FP) 0.5; }

int main(int argc, char **argv) {

	SeedGatherer sg1(point(0,0,0), 1.0);
	int i=0;
	while(!sg1.isDone()) {
		sg1.nextSeed();
		i++;
	}
	printf("%d Voronoi seeds returned by SeedGatherer\n", i);

	int h = 100;
	int w = 100;
	if(argc >= 3) {
		h = atoi(argv[1]);
		w = atoi(argv[2]);
	}

	float step = 0.05;

	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			point q;
			q.x = step*i; q.y = step*j; q.z = 5;
			point q2 = q; q2.z += step;
			point q3 = q; q3.z -= step;

			printf(voronoi_query(q) ? "#" : (voronoi_query(q2) || voronoi_query(q3) ? "." : " "));
		}
		printf("\n");
	}
	return 0;
}