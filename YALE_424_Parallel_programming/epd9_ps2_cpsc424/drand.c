#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#if __has_include ("omp.h")
#include <omp.h>
#else
#define omp_get_thread_num() (0)
#endif

static uint64_t seed;

void dsrand(unsigned s)
{
	seed = s-1 + omp_get_thread_num();
#ifdef VERBOSE
        printf("Seed = %lu. RAND_MAX = %d.\n",(long unsigned int)seed,RAND_MAX);
#endif
}

double drand(void)
{

	seed = 6364136223846793005ULL*seed + 1;
        return((double)(seed>>33)/(double)RAND_MAX);
}
