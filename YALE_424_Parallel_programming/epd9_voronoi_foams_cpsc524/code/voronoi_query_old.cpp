#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "voronoi_query.h"

#define EXCEPTION(str) { printf("line %d: %s\n", __LINE__, str); /* TODO do something to crash program*/ }

// #define ALLOCATE(type, count) (new type[count])
// #define DEALLOCATE(name) { delete [] name; }
#define ALLOCATE(type, count) ((type *)(malloc(sizeof(type) * (count))))
#define DEALLOCATE(name) {free(name); }


// TODO streamline pseudorandom()
/*
// Simple pseudorandom number between 0 and 1
// There are better methods, but this one is simple.
// Copied from https://stackoverflow.com/questions/11946622/implementation-of-random-number-generator
__host__ __device__
FP pseudorandom(FP s) {

	// Normally, these would be split into two functions,
	// But we don't want a real random number generator, so we reset the seed every step.
	unsigned int seed = (unsigned int)(s*100) & 0x7fffffffU;
    seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
	return seed / (FP) 0x7fffffffU;
}
*/

// Copied from https://stackoverflow.com/questions/11946622/implementation-of-random-number-generator
__host__ __device__
int rand (int newseed) {
	unsigned int seed = (unsigned)newseed & 0x7fffffffU;

    seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
    return (int)seed;
}
// Simple psuedorandom number between 0 and 1
__host__ __device__
FP pseudorandom(FP seed) {
	// seed^prime1 % prime2 is a simple randomish number generator
	// If you make prime1 or seed too big, bad things happen, so I made it repeat.
	//FP rand = (FP)fmod(powf(fmod(seed, 101), (float)17), (FP)9319) / (FP) 9319;

	FP random = rand((int) seed * 100) / (FP) 0x7fffffffU;
	return random;
}



__host__ __device__
FP euc_dist_squared(point p1, point p2) {
	FP x = p1.x-p2.x;
	FP y = p1.y-p2.y;
	FP z = p1.z-p2.z;
	return x*x + y*y + z*z;
}

__host__ __device__
FP dot(point p1, point p2) {
	return p1.x*p2.x + p1.y*p1.y + p1.z*p1.z;
}

__host__ __device__
point cross(point p1, point p2) {
	point c;
	c.x = p1.y*p2.z - p1.z*p2.y;
	c.y = p1.z*p2.x - p1.x*p2.z;
	c.z = p1.x*p2.y - p1.y*p2.x;
	return c;
}

__host__ __device__
point minus(point p1, point p2) {
	point p;
	p.x = p1.x-p2.x;
	p.y = p1.y-p2.y;
	p.z = p1.z-p2.z;
	return p;
}

__host__ __device__
point add(point p1, point p2) {
	point p;
	p.x = p1.x+p2.x;
	p.y = p1.y+p2.y;
	p.z = p1.z+p2.z;
	return p;
}

__host__ __device__
point random_sample_in_subcell(point c, FP l) {
	FP sum = c.x + c.y + c.z;
	c.x = c.x+(pseudorandom(2*c.x + sum)-0.5)*l;
	c.y = c.y+(pseudorandom(2*c.y + sum)-0.5)*l;
	c.z = c.z+(pseudorandom(2*c.z + sum)-0.5)*l;
	return c;
}

__host__ __device__
int randomize_subcell_ordering(point c, int i) {
	int j, ordering[8];
	for(j=0; j<8; j++) {
		ordering[j] = j;
	}
	for(j=0; j<8; j++) {
		int random = (int)(pseudorandom(c.x + j)*8);
		if(random >= 8) {
			EXCEPTION("Too large random number.");
		}

		int temp = ordering[j];
		ordering[j] = ordering[random];
		ordering[random] = temp;
	}
	return ordering[i];
}

// Get the ith subcell's center of cell q with size l.
__host__ __device__
point subcell(point q, FP l, int i) {
	point offsets[] = {
		point(1,1,1),
		point(1,1,-1),
		point(1,-1,1),
		point(1,-1,-1),
		point(-1,1,1),
		point(-1,1,-1),
		point(-1,-1,1),
		point(-1,-1,-1)
	};
	point p = q;
	p.x += l*offsets[i].x/4;
	p.y += l*offsets[i].y/4;
	p.z += l*offsets[i].z/4;
	return p;
}

__host__ __device__
int subdivide_cell(point q, FP l, point *seeds, int maxlen) {

	int i;
	FP target_seeds = rho(q) * (l*l*l);

	int seed_count = 0;

	if(target_seeds <= 8) {
		int exact_seeds;
		exact_seeds = (int) floor(target_seeds); // TODO should be floor(target_seeds) + random chance of +1
		if(pseudorandom(q.x+q.y+q.z) < target_seeds-exact_seeds) {
			exact_seeds += 1;
		}

		if(exact_seeds == 0) {
			exact_seeds = 1;
		}

		if(exact_seeds > maxlen) {
			exact_seeds = maxlen;
		}

		for(i=0; i<exact_seeds; i++) {
			seeds[i]  = random_sample_in_subcell(
				subcell(q,l,
					randomize_subcell_ordering(q, i)
				), l/2);
		}
		seed_count = exact_seeds;
	} else {
		// TODO make iterative...
		for(i=0; i<8; i++) {
			seed_count += subdivide_cell(subcell(q,l,i), l/2, seeds+seed_count, maxlen-seed_count);
		}
	}

	if(seed_count < 1) { EXCEPTION("Didn't create enough seeds in subdivide cell")}

	return seed_count;
}

// Probably maximum possible density.
__host__ __device__
int quick_density_estimate(point q, FP l) {
	int estimate = floor(2*rho(q) * (l*l*l));
	return estimate < 1 ? 1 : estimate; // A grid cell *must* have at least 1 seed.
}

// Get the closest grid cell center to point q.
__host__ __device__
point closest_center(point q, FP l) {
	q.x -= fmod(q.x,l);
	q.y -= fmod(q.y,l);
	q.z -= fmod(q.z,l);
	return q;
}

__host__ __device__
point *gather_seeds(point q, FP l, int *numSeeds) {
	int max_seeds = quick_density_estimate(q, l) * 125;
	int seeds_so_far = 0;
	point *seeds = ALLOCATE(point, max_seeds);
	if(seeds == NULL) {
		EXCEPTION("Error in seed allocation");
	}

	for(int i=0; i<5; i++) {
		for(int j=0; j<5; j++) {
			for(int k=0; k<5; k++) {
				point c(q.x+(i-2)*l, q.y+(j-2)*l, q.z+(k-2)*l);
				seeds_so_far += subdivide_cell(c,l,seeds+seeds_so_far, max_seeds - seeds_so_far);
			}
		}
	}
	*numSeeds = seeds_so_far;
	return seeds;
}

__host__ __device__
point closest_point_to_bisector_line(point q, point s1, point s2, point s3) {

	point axis = cross(minus(s1, s2), minus(s1, s3));
	if(fabs(axis.x) < 0.0001 && fabs(axis.y) < 0.0001 && fabs(axis.z) < 0.0001) {
		// Seeds are colinear.
		point p; p.x = INFINITY; p.y = INFINITY; p.z = INFINITY;
		return p;
	}
	point avg_seed;
	avg_seed.x = (s1.x + s2.x + s3.x)/ (FP)3;
	avg_seed.y = (s1.y + s2.y + s3.y)/ (FP)3;
	avg_seed.z = (s1.z + s2.z + s3.z)/ (FP)3;

	// Find the closest point:
	FP dotted = dot(axis, minus(q, avg_seed));
	point closest;
	closest.x = dotted * axis.x + avg_seed.x;
	closest.y = dotted * axis.y + avg_seed.y;
	closest.z = dotted * axis.z + avg_seed.z;

	return closest;
}

__host__ __device__
VOX eval_structure(point q, FP l, point *seeds, int num_seeds) {
	FP tau = BEAM_RAD;

	int closest_seed_i = 0;
	FP closest_seed_dist = euc_dist_squared(q, seeds[closest_seed_i]);
	for(int i=1; i<num_seeds; i++) {
		FP dist = euc_dist_squared(q, seeds[i]);
		if(dist < closest_seed_dist) {
			closest_seed_i = i;
			closest_seed_dist = dist;
		}
	}
	point closest_seed = seeds[closest_seed_i];
	seeds[closest_seed_i] = seeds[0];
	seeds[0] = closest_seed; // Not necessary.


	for(int i=1; i<num_seeds-1; i++) { // Skip the closest seed and the last seed
		for(int j=i+1; j<num_seeds; j++) {
			point pl = closest_point_to_bisector_line( q, closest_seed, seeds[i], seeds[j]);

			if(euc_dist_squared(q, pl) <= tau*tau) {

				bool accept = true;
				for(int k=1; k<num_seeds; k++) {
					if(euc_dist_squared(closest_seed, pl) > euc_dist_squared(seeds[k], pl)) {
						accept = false;
						break;
					}
				}
				if(accept) return (VOX) true;
			}
		}
	}
	return (VOX) false;
}

__host__ __device__
VOX voronoi_query(point q) {
	FP l = CELL_SIZE; int num_seeds;

	point *seeds = gather_seeds(q, l, &num_seeds);
	// num_seeds = 3;
	// point seeds[3] = { {1,1,0}, {-1,1,0}, {0,-2,0} };

	if(num_seeds < 125) { printf("%d\n", num_seeds);EXCEPTION("Too few seeds created");}

	VOX in_object = eval_structure(q, l, seeds, num_seeds);
	DEALLOCATE(seeds);
	return in_object;
}



