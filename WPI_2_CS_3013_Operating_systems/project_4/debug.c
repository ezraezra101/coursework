// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems



#define DEBUG_TAB "" //"\t\t"


// http://en.wikipedia.org/wiki/Offsetof
#define offsetof(st, m) __builtin_offsetof(st, m)
#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})

#include "header.h"
extern pthread_mutex_t eviction_lock;

int debug_mutex_lock(pthread_mutex_t *lock) {
	if((pte_entry *)lock > page_table &&
			(pte_entry *) lock < page_table + 1000) {
		printf(DEBUG_TAB "lock page %d\n",
		container_of(lock, pte_entry, locked)-page_table);
	}else if(lock == &eviction_lock) {
		printf(DEBUG_TAB "lock eviction_lock\n");
	} else {
		printf(DEBUG_TAB "lock\n");
	}
	return pthread_mutex_lock(lock);
}

int debug_mutex_unlock(pthread_mutex_t *lock) {
	if((pte_entry *)lock > page_table &&
			(pte_entry *) lock < page_table + 1000) {
		printf(DEBUG_TAB "unlock page %d\n",
		container_of(lock, pte_entry, locked)-page_table);
	}else if(lock == &eviction_lock) {
		printf(DEBUG_TAB "unlock eviction_lock\n");
	} else {
		printf(DEBUG_TAB "unlock\n");
	}
	
	return pthread_mutex_unlock(lock);
}

int debug_mutex_trylock(pthread_mutex_t *lock) {
	int success = pthread_mutex_trylock(lock);
	
	if((pte_entry *)lock > page_table &&
			(pte_entry *) lock < page_table + 1000) {
				
		printf(DEBUG_TAB " trylock%s page %d\n", success ? "(fail)":"",
		container_of(lock, pte_entry, locked)-page_table);
	} else if(lock == &eviction_lock) {
		printf(DEBUG_TAB "trylock%s eviction_lock\n", success ? "(fail)":"");
	} else {
		printf(DEBUG_TAB "trylock%s\n", success ? "(fail)":"");
	}
	
	return success;
}

void print_locked_pages() {
	int i;
	
	// pthread_mutex_trylock(&eviction_lock);
	
	printf("Printing all currently locked pages:\n");
	
	for (i=0; i<1000; i++) {
		if(pthread_mutex_trylock(&page_table[i].locked)) {
			printf("%d locked (%c)\n", i,
			  page_table[i].ram_addr == UNALLOCATED ? 'U' : 'r');
		} else {
			pthread_mutex_unlock(&page_table[i].locked);
		}
	}
}


