// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems

#ifndef MEM_LIB
#define MEM_LIB

typedef signed short vAddr;

typedef enum {
	FIFO, CLOCK, AGING
} eviction_algorithm_t;

// Reserves a new memory location sizeof(int) in "RAM"
// Returns -1 if no memory is available
vAddr allocateNewInt();

int *accessIntPtr(vAddr address);

void unlockMemory(vAddr address);

void freeMemory(vAddr address);

void options(int timeScale, eviction_algorithm_t evictAlg);

#endif
