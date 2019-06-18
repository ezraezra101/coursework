// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems

#include <pthread.h>

#include "header.h"

#define SHIFT_OVER (sizeof(int) * 8 - 1)

static unsigned int ram_ages[25] = {0};
static unsigned int ssd_ages[100] = {0};

int aging_evict_from_ram() {
  unsigned int oldest_age = -1;
	int isLocked, i,
      page_frame = UNALLOCATED;
  
	for (i=0; i<25; i++) {
    isLocked = pthread_mutex_trylock(&page_table[ram_vAddrs[i]].locked);
    if (!isLocked) {
      pthread_mutex_unlock(&page_table[ram_vAddrs[i]].locked);
			
			if(ram_ages[i] <= oldest_age) { //Note that smaller numbers indicate older pages
				oldest_age = ram_ages[i];
				page_frame = i;
			}
    }
	}
	
	ram_ages[page_frame] = 0;
	
	return page_frame;
}


int aging_evict_from_ssd() {
  unsigned int oldest_age = -1;
	int isLocked, i,
      page_frame = UNALLOCATED;
  
	for (i=0; i<100; i++) {
    isLocked = pthread_mutex_trylock(&page_table[ssd_vAddrs[i]].locked);
    if (!isLocked) {
      pthread_mutex_unlock(&page_table[ssd_vAddrs[i]].locked);
			
			if(ssd_ages[i] <= oldest_age) { //Note that smaller numbers indicate older pages
				oldest_age = ssd_ages[i];
				page_frame = i;
			}
    }
	}
	
	ssd_ages[page_frame] = 0;
	
	return page_frame;
}


void aging_update(vAddr address) {
  pte_entry *entry = page_table + address;
  
  int i;
  
  if (entry->ram_addr != UNALLOCATED) {
  	for (i=0; i<25; i++) {
  		ram_ages[i] = ram_ages[i] >> 1;
  	}
    ram_ages[entry->ram_addr] |= 1 << SHIFT_OVER;
  }
  
  if (entry->ssd_addr != UNALLOCATED) {
  	for (i=0; i<100; i++) {
  		ssd_ages[i] = ssd_ages[i] >> 1;
  	}
    ssd_ages[entry->ssd_addr] |= 1 << SHIFT_OVER;
  }
}
