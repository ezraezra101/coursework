// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems
#include <pthread.h>

#include "header.h"

int fifo_evict_from_ram() {
  static int count = 0;
  int evicted_page_frame;
  vAddr evicted_page;
  
  do {
    evicted_page_frame = count++ % 25;
    evicted_page = ram_vAddrs[evicted_page_frame];
    
  } while (pthread_mutex_trylock(&page_table[evicted_page].locked)
    && evicted_page != UNALLOCATED);
  
  pthread_mutex_unlock(&page_table[evicted_page].locked);
  
  return evicted_page_frame;
}

int fifo_evict_from_ssd() {
  static int count = 0;
  int evicted_page_frame;
  vAddr evicted_page;
  
  do {
    evicted_page_frame = count++ % 100;
    evicted_page = ssd_vAddrs[evicted_page_frame];
    
  } while (pthread_mutex_trylock(&page_table[evicted_page].locked)
    && evicted_page != UNALLOCATED);
  
  pthread_mutex_unlock(&page_table[evicted_page].locked);
  
  return evicted_page_frame;
}
