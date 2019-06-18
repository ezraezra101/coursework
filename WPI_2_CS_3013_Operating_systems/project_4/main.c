// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "debug.h"
#include "mem_lib.h"

// Number of microseconds per virtual second for our testing simulation
#define TIME_SCALE 100

///////////////////////
//   Simple Tests    //
///////////////////////

void test_simple_allocate() {
	vAddr var_loc = allocateNewInt();
	int *var = accessIntPtr(var_loc);
	
	*var = 5;
	
	unlockMemory(var_loc);
	
	var = accessIntPtr(var_loc);
		
	if(*var != 5) {
		printf("Error! Variable changed illegally!\n");
	}
	
	unlockMemory(var_loc);
	
	freeMemory(var_loc);
}

void test_allocate_until_swap() {
	vAddr var_locs[26];
	int i, *var;
	for(i=0; i<26; i++) {
		var_locs[i] = allocateNewInt();
		var = accessIntPtr(var_locs[i]);
		*var = i;
		unlockMemory(var_locs[i]);
	}
	
	for(i=0; i<26; i++) {
		var = accessIntPtr(var_locs[i]);
		if(*var != i) {
			printf("Error! Variable changed :-(\n");
		}
		freeMemory(var_locs[i]);
	}
}

void test_allocate_until_swap_to_disk() {
	vAddr var_locs[126];
	int i, *var;
	for(i=0; i<126; i++) {
		var_locs[i] = allocateNewInt();
		var = accessIntPtr(var_locs[i]);
		*var = i;
		unlockMemory(var_locs[i]);
	}
	
	for(i=0; i<126; i++) {
		var = accessIntPtr(var_locs[i]);
		if(*var != i) {
			printf("Error! Variable changed :-(\n");
		}
		freeMemory(var_locs[i]);
	}
}

void memoryMaxer() {
	vAddr indexes[1000];
  int i;
	for (i = 0; i < 1000; i++) {
		indexes[i] = allocateNewInt();
		int *value = accessIntPtr(indexes[i]);
		*value = (i * 3);
		unlockMemory(indexes[i]);
	}
	for (i = 0; i < 1000; i++) {
		freeMemory(indexes[i]);
	}
}


void test_freeing() {
	int i;
	for(i=0; i< 2000; i++) {
		vAddr address = allocateNewInt();
		
		if(address == -1) {
			printf("Unable to allocate memory :-(\n");
		}
		
		freeMemory(address);
	}
}

void test_too_much_mem() {
	int i;
	vAddr indexes[1000];
	for(i=0; i< 1000; i++) {
		indexes[i] = allocateNewInt();
		
		if(indexes[i] == -1) {
			printf("Unable to allocate memory :-(\n");
		}
	}
	
	if(allocateNewInt() != -1) {
		printf("More than 1000 virtual addresses!\n");
	}
	
	for(i=0; i<1000; i++) {
		freeMemory(indexes[i]);
	}
}

void test_allocate_when_ram_locked() {
	int i;
	vAddr indexes[25];
	for(i=0; i< 25; i++) {
		indexes[i] = allocateNewInt();
    accessIntPtr(indexes[i]); // Lock the page in RAM
		
		if(indexes[i] == -1) {
			printf("Unable to allocate memory :-(\n");
		}
	}
	
	if(allocateNewInt() != -1) {
		printf("Allowed page to be allocated with fully locked RAM! :-(\n");
	}
	
	for(i=0; i<25; i++) {
		freeMemory(indexes[i]);
	}
}


///////////////////////
// Normal functions  //
///////////////////////

int test_function_runtime(void (*test)()) {
  struct timeval start_time, end_time;
  gettimeofday(&start_time, NULL);
  test();
  gettimeofday(&end_time, NULL);
  return (end_time.tv_sec - start_time.tv_sec) * 1000000
      + (end_time.tv_usec - start_time.tv_usec);
}

void test_battery(eviction_algorithm_t evict_alg) {
  printf("Note that all times given below are relative to their time scales.\n"
         "The theoretical times are based on the optimal page eviction algorithm.\n\n");
	options(TIME_SCALE, evict_alg);
  
  // Converts real microseconds to virtual seconds
  //  1/1000000 * 10000000/MICROSECONDS_PER_VIRTUAL_SECOND
  // 1/MICROSECONDS_PER_VIRTUAL_SECOND
  float time_conversion = 1/(float)TIME_SCALE;

	printf("\tTesting simple allocation; it should finish in negligible time\n");
	printf("\tIt actually took %f seconds\n",
    test_function_runtime(test_simple_allocate) * time_conversion);
  
  // Involves a single SSD read + write: 2 * 0.25 seconds = 0.5 seconds
	printf("\tTesting swapping to ssd; it should take at least 0.5 seconds\n");
	printf("\tIt actually took %f seconds\n",
    test_function_runtime(test_allocate_until_swap) * time_conversion);
  
  // Theoretical time: swapping to SSD after the first 25 and swapping to disk
  //  for the final allocation. Because we create all the pointers and then read
  //  them all, this time is doubled. Therefore 2 * (101 * 0.25 + 2.5) = 55.5
	printf("\tTesting swapping to disk; it should take at least 55.5 seconds\n");
	printf("\tIt actually took %f seconds\n",
    test_function_runtime(test_allocate_until_swap_to_disk) * time_conversion);
    
  // Theoretical time: swapping to SSD after the first 25 and swapping to
  //  disk after the first 125. Therefore 975 * 0.25 + 875 * 2.5 = 2431.25
	printf("\tTesting maximum memory; it should take at least 2431.25 seconds\n");
	printf("\tIt actually took %f seconds\n",
    test_function_runtime(memoryMaxer) * time_conversion);
	
  // Theoretical time: swapping to SSD after the first 25 and swapping to
  //  disk after the first 125. Therefore 975 * 0.25 + 875 * 2.5 = 2431.25
	printf("\tTesting memory overflow; it should take at least 2431.25 seconds\n");
	printf("\tIt actually took %f seconds\n",
    test_function_runtime(test_too_much_mem) * time_conversion);
	
	printf("\tTesting allocation with locked RAM; it should finish in negligible time\n");
	printf("\tIt actually took %f seconds\n",
    test_function_runtime(test_allocate_when_ram_locked) * time_conversion);
  
	options(1000000, evict_alg);
	
	//Non-fast tests go here...
	printf("\tTests freeing memory; it should finish in negligible time\n");
	printf("\tIt actually took %f seconds\n",
    test_function_runtime(test_freeing)/1000000.0);
}


void *thread_function(void *id) {
	// usleep(rand() % 10000);
	
	test_simple_allocate();
	
	printf("."); fflush(stdout);
	
	return NULL;
}

void read_in_arguments(int argc, char ** argv,
        int *thread_count, eviction_algorithm_t *evict_alg) {
	int i;
	*evict_alg = CLOCK;
	*thread_count = 20;
	
	for (i=1; i<argc; i++) {
		if(0 == strncmp(argv[i], "clock", 5)) {
			*evict_alg = CLOCK;
		} else if(0 == strncmp(argv[i], "aging", 5)) {
			*evict_alg = AGING;
		} else if(0 == strncmp(argv[i], "fifo", 4)) {
			*evict_alg = FIFO;
		} else {
			int read_count = atoi(argv[i]);
			*thread_count = read_count > 0 ? read_count : *thread_count;
		}
	}
	return;
}


int main(int argc, char ** argv) {
	
	int thread_count;
	eviction_algorithm_t evict_alg;
	read_in_arguments(argc, argv, &thread_count, &evict_alg);
	
	printf("%s eviction algorithm selected.\n\n",
		evict_alg == FIFO ? "Modified fifo" :
		evict_alg == CLOCK ? "Clock" : "Aging" );
	
	printf("Testing threads (with %d threads)\n", thread_count);
	
	options(TIME_SCALE, evict_alg);
	
  pthread_t *threads = alloca(thread_count * sizeof(pthread_t));
  long i;
  for (i = 0; i < thread_count; i++) {
    int status = pthread_create(&threads[i], NULL, thread_function, (void *) i);
    if (status != 0) { // if status is not 0, an error occurred!
      printf("Error in pthread_create: %d", errno);
      exit(errno);
    }
  }
	
	//DEBUG
	// sleep(10);
	// print_locked_pages();
	
	for (i = 0; i< thread_count; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("\nNon-thread related tests\n");
	test_battery(evict_alg);

	return 0;
}
