// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems

#include <pthread.h>

#include "header.h"

static int ram_clock[25] = {0}; //creates array of 25 0's
static int ssd_clock[100] = {0};
static int ram_clock_hand = -1;
static int ssd_clock_hand = -1;


int clock_evict_from_ram() {
  int isLocked, recentlyUsed;
  do {
    ram_clock_hand = (ram_clock_hand + 1) % 25;
    
    isLocked = pthread_mutex_trylock(&page_table[ram_vAddrs[ram_clock_hand]].locked);
    if (!isLocked) {
      pthread_mutex_unlock(&page_table[ram_vAddrs[ram_clock_hand]].locked);
    }
    
    recentlyUsed = ram_clock[ram_clock_hand];
    ram_clock[ram_clock_hand] = FALSE;
    
  } while (isLocked || recentlyUsed);
	
  return ram_clock_hand;
}


int clock_evict_from_ssd() {
  int isLocked, recentlyUsed;
  do {
    ssd_clock_hand = (ssd_clock_hand + 1) % 100;
    
    isLocked = pthread_mutex_trylock(&page_table[ssd_clock_hand].locked);
    if (!isLocked) {
      pthread_mutex_unlock(&page_table[ssd_clock_hand].locked);
    }
    
    recentlyUsed = ssd_clock[ssd_clock_hand];
    ssd_clock[ssd_clock_hand] = FALSE;
    
  } while (isLocked || recentlyUsed);
  
  return ssd_clock_hand;
}


void clock_update(vAddr address) {
  pte_entry *entry = page_table + address;
  if (entry->ram_addr != UNALLOCATED) {
    ram_clock[entry->ram_addr] = TRUE;
  }
  if (entry->ssd_addr != UNALLOCATED) {
    ssd_clock[entry->ssd_addr] = TRUE;
  }
}
