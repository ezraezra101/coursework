// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems

#ifndef PRIVATE_MODULE_HEADER_INCLUDED
#define PRIVATE_MODULE_HEADER_INCLUDED

#include "mem_lib.h"

#define TRUE -1
#define FALSE 0

#define UNALLOCATED -1

#if __APPLE__
#define pthread_yield() usleep(1)
#warning "container_of macro returns wrong type for us..."
#endif


typedef enum {
  RAM, SSD, DISK
} storage_t;

typedef struct {
  int ram_addr; // Offset, 0-24 or UNALLOCATED
  int ssd_addr; // Offset, 0-99 or UNALLOCATED
  int disk_addr;// Offset, 0-999 or UNALLOCATED
  int ssd_dirty;// TRUE or FALSE
  pthread_mutex_t locked;
  
  // ram_dirty doesn't exist because we assume
  //    that whenever a 'page' is loaded into
  //    ram, it will be modified. This is because
  //    we cannot tell the difference between
  //    accessing a page and modifying it.
  
  // Need read bit/aging int or stuff?
  
} pte_entry;

extern pte_entry page_table[1000];
extern vAddr ram_vAddrs[25];
extern vAddr ssd_vAddrs[100];

#endif