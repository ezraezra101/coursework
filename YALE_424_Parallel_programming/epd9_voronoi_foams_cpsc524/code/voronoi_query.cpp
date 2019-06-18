#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "voronoi_query.h"

#define EXCEPTION(str) { printf("line %d: %s\n", __LINE__, str); /* TODO do something to crash program*/ }

#define ALLOCATE(type, count) (new type[count])
#define DEALLOCATE(name) { delete [] name; }
// #define ALLOCATE(type, count) ((type *)(malloc(sizeof(type) * (count))))
// #define DEALLOCATE(name) {free(name); }


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

class SeedGatherer {
public: // Probably should be private
	enum { MAX_SEEDS = 5 }; // The maximum number of seeds in a grid cell.
	int cur_cell; // The current grid cell in the two ring neighborhood
	int cur_seed; // We are interested in the `i`th seed in the current grid cell
	int n_seeds;  // Total number of seeds in the current grid cell
	point q_center;
	FP l;
	point cellSeeds[MAX_SEEDS];


	__host__ __device__ int i() { return (cur_cell/25) % 5; };
	__host__ __device__ int j() { return (cur_cell/5) % 5; };
	__host__ __device__ int k() { return cur_cell % 5; };

	__host__ __device__
	point center() { return point(q_center.x+(i()-2)*l, q_center.y+(j()-2)*l, q_center.z+(k()-2)*l); }
public:
	__host__ __device__
	SeedGatherer(point q, FP l) {
		cur_cell = -1;
		cur_seed = 0;
		n_seeds = 0;
		q_center = closest_center(q, l);
		this->l = l;
	}

	__host__ __device__
	SeedGatherer(const SeedGatherer &other) {
		cur_cell = other.cur_cell;
		cur_seed = other.cur_seed;
		l = other.l;
		q_center = other.q_center;
		n_seeds = cur_cell == -1 ? 0 : subdivide_cell(center(), l, cellSeeds, MAX_SEEDS);
	}

	__host__ __device__
	bool isDone() {
		return cur_cell == 5*5*5-1 && cur_seed == n_seeds;
	}

	__host__ __device__
	point nextSeed() {
		if(isDone()) {
			return point(INFINITY, INFINITY, INFINITY);
		}
		// If we need to recalculate the seeds in cellSeeds[], move to the next cell & recalculate.
		if(n_seeds <= cur_seed) {
			cur_cell++;
			cur_seed = 0;
			n_seeds = subdivide_cell(center(), l, cellSeeds, MAX_SEEDS);

			if(n_seeds == MAX_SEEDS) {
				// TODO complain that things might be too dense
			}
		}

		// printf("Seed gatherer: %d %d %d, seed %d. \n", i(),j(), k(), cur_seed);

		return cellSeeds[cur_seed++];
	}
};



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
VOX eval_structure(point q, FP l) {
	FP tau = BEAM_RAD;

	SeedGatherer sg = SeedGatherer(q,l);

	point closest_seed = sg.nextSeed();
	FP closest_seed_dist = euc_dist_squared(q, closest_seed);
	while(!sg.isDone()) {
		point s = sg.nextSeed();
		FP dist = euc_dist_squared(q, s);
		if(dist < closest_seed_dist) {
			closest_seed = s;
			closest_seed_dist = dist;
		}

	}

	sg = SeedGatherer(q,l);
	while(!sg.isDone()) {
		point seed_1 = sg.nextSeed();
		SeedGatherer sg2 = sg;
		while(!sg2.isDone()) {
			point seed_2 = sg2.nextSeed();
			point pl = closest_point_to_bisector_line( q, closest_seed, seed_1, seed_2);

			if(euc_dist_squared(q, pl) <= tau*tau) {
				bool accept = true;

				SeedGatherer sg3 = SeedGatherer(q,l);
				while(!sg3.isDone()) {
					point seed_3 = sg3.nextSeed();
					if(euc_dist_squared(closest_seed, pl) > euc_dist_squared(seed_3, pl)) {
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
	FP l = CELL_SIZE;
	VOX in_object = eval_structure(q, l);
	return in_object;
}



