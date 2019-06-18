// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "alg_aging.h"
#include "alg_clock.h"
#include "alg_fifo.h"
#include "header.h"

#include "debug.c"

#ifdef DEBUG
#define pthread_mutex_lock(lock)    printf("\t\t%d ",__LINE__); debug_mutex_lock(lock)
#define pthread_mutex_unlock(lock)  printf("\t\t%d ",__LINE__); debug_mutex_unlock(lock)
#define pthread_mutex_trylock(lock) debug_mutex_trylock(lock)

#endif

int get_ram_addr(vAddr address);
int get_ssd_addr(vAddr address);


int time_scale = 1000000;

int ram[25];
int ssd[100];
int disk[1000];

pte_entry page_table[1000];

vAddr ram_vAddrs[25];
vAddr ssd_vAddrs[100];

pthread_mutex_t eviction_lock;

eviction_algorithm_t eviction_algorithm = FIFO;

//////////////////////////
// "Hardware" Functions //
//////////////////////////

void segfault(int offset) {
  printf("Bus error! %d not in memory!\n", offset);
  exit(1);
}

int *access_ram(int offset) {
  if(offset >= 25 || offset < 0) {
    segfault(offset);
  }
  return ram + offset;
}

int *access_ssd(int offset) {
  if(offset >= 100 || offset < 0) {
    segfault(offset);
  }
  usleep(time_scale/4);
  return ssd + offset;
}

int *access_disk(int offset) {
  if(offset >= 1000 || offset < 0) {
    segfault(offset);
  }
  usleep(time_scale/2 * 5);
  return disk + offset;
}

void init() {
  pthread_mutex_init(&eviction_lock, NULL);
  int i;
  for(i=0; i<1000; i++) {
    page_table[i].ram_addr  = UNALLOCATED;
    page_table[i].ssd_addr  = UNALLOCATED;
    page_table[i].disk_addr = UNALLOCATED;
    pthread_mutex_init(&page_table[i].locked, NULL);
  }
  
  for(i=0; i<25; i++) {
    ram_vAddrs[i] = UNALLOCATED;
  }
  
  for(i=0; i<100; i++) {
    ssd_vAddrs[i] = UNALLOCATED;
  }
}

// Finds a new vAddr and gets a spot for it on the disk
vAddr find_new_vAddr() {
  pthread_mutex_lock(&eviction_lock);
  vAddr i;
  vAddr address = UNALLOCATED;
  for(i=0; i<1000; i++) {
    if((vAddr) UNALLOCATED == page_table[i].disk_addr) {
      page_table[i].disk_addr = i;
      address = i;
      break;
    }
  }
  pthread_mutex_unlock(&eviction_lock);
  //Remains UNALLOCATED if no free vAddrs.
  return address;
}

void validate_vAddr(vAddr address) {
  if(address == UNALLOCATED) {
    printf("Memory access exception - 'null' pointer exception.\n");
    exit(1);
  } else if((unsigned int) address >= 1000) {
    printf("Memory access exception - illegal virtual address.\n");
    exit(1);
  }
}

// Swaps page from RAM to the SSD
// Expects the address to be locked, and does not unlock it.
// Doesn't change ram_vAddrs[]
void copy_ram_to_ssd(vAddr address) {
  if(address == UNALLOCATED) {
    printf("Called copy_ram_to_ssd on address not in ram!\n");
    return;
  }
  
  pte_entry *entry = page_table + address;
	
  if(UNALLOCATED == entry->ram_addr) {
    // If the slot is unallocated in RAM
    return;
  } else if(UNALLOCATED == entry->ssd_addr) {
    entry->ssd_addr = get_ssd_addr(address);
  }
  
	if(entry->ram_addr == UNALLOCATED) {
		printf("Ram address somehow got removed: vAddr: %d, ram_addr: %d, ssd_addr: %d\n", address, entry->ram_addr, entry->ssd_addr);
		print_locked_pages();
		exit(45);
	}
	
  int *ssd_val = access_ssd(entry->ssd_addr);
	
	
  *ssd_val = *access_ram(entry->ram_addr);
  entry->ssd_dirty = TRUE;
}

// Swaps page from the SSD to the disk
// Expects the address to be locked, and does not unlock it.
// Doesn't change ssd_vAddrs[]
void copy_ssd_to_disk(vAddr address) {
  if(address == UNALLOCATED) {
    return;
  }
  
  pte_entry *entry = page_table + address;
  
  if(UNALLOCATED == entry->ssd_addr) {
    // If the slot is unallocated in SSD
    return;
  }

  int *disk_val = access_disk(entry->disk_addr);
  *disk_val = *access_ssd(entry->ssd_addr);
  entry->ssd_dirty = FALSE;
}

void cache_disk_to_ssd(vAddr address) {
  int ssd_address = get_ssd_addr(address);
  
  ssd[ssd_address] = *access_disk(page_table[address].disk_addr);
}

void cache_ssd_to_ram(vAddr address) {
  int ram_address = get_ram_addr(address);

  if(page_table[address].ssd_addr == UNALLOCATED) {
    cache_disk_to_ssd(address);
  }
  
  ram[ram_address] = *access_ssd(page_table[address].ssd_addr);
}

//////////////////////////
// Eviction Algorithm   //
// dependent functions  //
//////////////////////////

int find_unallocated_ram() {
  int i;
  for (i=0; i<25; i++) {
    if (ram_vAddrs[i] == UNALLOCATED) {
      return i;
    }
  }
  return UNALLOCATED;
}

int no_unlocked_ram() {
	int i;
	for(i=0; i<25; i++) {
		if(ram_vAddrs[i] == UNALLOCATED) {
			return FALSE;
		} else if(!pthread_mutex_trylock(&page_table[ram_vAddrs[i]].locked)) {
			pthread_mutex_unlock(&page_table[ram_vAddrs[i]].locked);
			return FALSE;
		}
	}
	return TRUE;
}

int find_unallocated_ssd() {
  int i;
  for (i=0; i<100; i++) {
    if (ssd_vAddrs[i] == UNALLOCATED) {
      return i;
    }
  }
  return UNALLOCATED;
}

int no_unlocked_ssd() {
	int i;
	for(i=0; i<100; i++) {
		if(ssd_vAddrs[i] == UNALLOCATED) {
			return FALSE;
		} else if(!pthread_mutex_trylock(&page_table[ssd_vAddrs[i]].locked)) {
			pthread_mutex_unlock(&page_table[ssd_vAddrs[i]].locked);
			return FALSE;
		}
	}
	return TRUE;
}


// Finds a page frame in RAM and puts the page at address in the frame.
// If necessary, evicts a page from RAM based on our page-replacement algorithm
int get_ram_addr(vAddr address) {


  pthread_mutex_lock(&eviction_lock);
	
  //Algorithm
  int evicted_page_frame;
	
	if(no_unlocked_ram()) {
		pthread_mutex_unlock(&eviction_lock);
		return UNALLOCATED;
	}
	
	evicted_page_frame = find_unallocated_ram();
	
  if (evicted_page_frame == UNALLOCATED) {
    switch(eviction_algorithm) {
    case FIFO:
      evicted_page_frame = fifo_evict_from_ram();
      break;
    case CLOCK:
      evicted_page_frame = clock_evict_from_ram();
      break;
		case AGING:
			evicted_page_frame = aging_evict_from_ram();
			break;
    default:
      printf("Illegal eviction algorithm!\n");
      exit(1);
    }
  }
  vAddr evicted_page = ram_vAddrs[evicted_page_frame];

  if(evicted_page != UNALLOCATED) {
    pthread_mutex_lock(&page_table[evicted_page].locked);
  }

  ram_vAddrs[evicted_page_frame] = address;
  
  // End algorithm
  
  pthread_mutex_unlock(&eviction_lock);
  
  page_table[address].ram_addr = evicted_page_frame;
  
  //Clean up evicted page
  if(evicted_page != UNALLOCATED) {
    //Swap RAM to SSD (Not in critical region!)
    copy_ram_to_ssd(evicted_page);
    
    pthread_mutex_lock(&eviction_lock);
    page_table[evicted_page].ram_addr = UNALLOCATED;
    pthread_mutex_unlock(&page_table[evicted_page].locked);
    pthread_mutex_unlock(&eviction_lock);
  }
  
  return evicted_page_frame;
}

// Finds a page frame in RAM and puts the page at address in the frame.
// If necessary, evicts a page from RAM based on our page-replacement algorithm
int get_ssd_addr(vAddr address) {

  pthread_mutex_lock(&eviction_lock);
	
  //Algorithm
  int evicted_page_frame;
  
  // There should always be unlocked memory on the SSD
  // Up to 25 operations may be occurring simultaneously (each needs a spot in RAM)
  // each of these operations can affect up to 3 pieces in memory:
  //  The page being promoted to RAM,
  //  The page being demoted to the SSD,
  //  and possibly the page being demoted from SSD to disk.
  // But it doesn't hurt to have this check here anyway (much).
	while(no_unlocked_ssd()) {
		pthread_mutex_unlock(&eviction_lock);
		pthread_yield();
		pthread_mutex_lock(&eviction_lock);
	}
	
  evicted_page_frame = find_unallocated_ssd();
	
  if (evicted_page_frame == UNALLOCATED) {
    switch(eviction_algorithm) {
    case FIFO:
      evicted_page_frame = fifo_evict_from_ssd();
      break;
    case CLOCK:
      evicted_page_frame = clock_evict_from_ssd();
      break;
		case AGING:
			evicted_page_frame = aging_evict_from_ssd();
			break;
    default:
      printf("Illegal eviction algorithm!\n");
      exit(1);
    }
  }
	
  vAddr evicted_page = ssd_vAddrs[evicted_page_frame];

  if(evicted_page != UNALLOCATED) {
    pthread_mutex_lock(&page_table[evicted_page].locked);
  }

  ssd_vAddrs[evicted_page_frame] = address;
  
  // End algorithm
  
  pthread_mutex_unlock(&eviction_lock);
  
  page_table[address].ssd_addr = evicted_page_frame;
  
  //Clean up evicted page
  if(evicted_page != UNALLOCATED) {
    //Swap RAM to SSD (Not in critical region!)
    copy_ssd_to_disk(evicted_page);
    
    pthread_mutex_lock(&eviction_lock);
    page_table[evicted_page].ssd_addr = UNALLOCATED;
    pthread_mutex_unlock(&page_table[evicted_page].locked);
    pthread_mutex_unlock(&eviction_lock);
  }
  
  return evicted_page_frame;
}

void update(vAddr accessed_page) {
  switch(eviction_algorithm) {
  case FIFO:
    break;
  case CLOCK:
    clock_update(accessed_page);
    break;
	case AGING:
		aging_update(accessed_page);
		break;
  default:
    printf("Illegal eviction algorithm!\n");
    exit(1);
  }
}


//////////////////////
// Public functions //
//////////////////////

vAddr allocateNewInt() {
  static pthread_once_t is_initialized = PTHREAD_ONCE_INIT;
  pthread_once(&is_initialized, &init);
  
  vAddr address = find_new_vAddr();
	
  if(address != UNALLOCATED) {
		pthread_mutex_lock(&page_table[address].locked);
		
    int ram_address = get_ram_addr(address);
    if (ram_address == UNALLOCATED) {
      freeMemory(address);
      address = UNALLOCATED;
    } else {
      ram_vAddrs[ram_address] = address;

      page_table[address].ram_addr = ram_address;
	
    	pthread_mutex_unlock(&page_table[address].locked);
    }
  }
  pthread_mutex_unlock(&eviction_lock);
  return address;
}

//Note: Blocking function.
// Multiple threads cannot access the same page.
// Don't call it twice :-(
int *accessIntPtr(vAddr address) {
  validate_vAddr(address);

	// Spin until the address can be locked
	// Allows other processes to enter critical
	//   region when waiting for lock
  pthread_mutex_lock(&eviction_lock);
	while(pthread_mutex_trylock(&page_table[address].locked)) {
  	pthread_mutex_unlock(&eviction_lock);
		pthread_yield();
		pthread_mutex_lock(&eviction_lock);
	}
	pthread_mutex_unlock(&eviction_lock);
	

  if(page_table[address].ram_addr == UNALLOCATED) {
    cache_ssd_to_ram(address);
  }

  return access_ram(page_table[address].ram_addr);
}

void unlockMemory(vAddr address) {
  validate_vAddr(address);
  
  pthread_mutex_lock(&eviction_lock);
  
  pthread_mutex_unlock(&page_table[address].locked);
  // Update eviction-related variables
  update(address);
  
  pthread_mutex_unlock(&eviction_lock);
  return;
}

void freeMemory(vAddr address) {
  if(address >= 1000 || address < 0) {
    return;
  }
  
  pthread_mutex_lock(&eviction_lock);
  
  // Tries to lock memory just in case you've
  //   called unlockMemory after accessIntPtr
  pthread_mutex_trylock(&page_table[address].locked);
  
  if(UNALLOCATED != page_table[address].ram_addr) {
    ram_vAddrs[page_table[address].ram_addr] = UNALLOCATED;
  }
  if(UNALLOCATED != page_table[address].ssd_addr) {
    ssd_vAddrs[page_table[address].ssd_addr] = UNALLOCATED;
  }
  
  page_table[address].ram_addr = UNALLOCATED;
  page_table[address].ssd_addr  = UNALLOCATED;
  page_table[address].disk_addr = UNALLOCATED;
  page_table[address].ssd_dirty = FALSE;
  pthread_mutex_unlock(&page_table[address].locked);
  
  pthread_mutex_unlock(&eviction_lock);
  
  return;
}


void options(int timeScale, eviction_algorithm_t evictAlg) {
  time_scale = timeScale;
  eviction_algorithm = evictAlg;
  printf("Now running at %.3f times normal speed, using %s algorithm\n",
    1000000.0/timeScale,
    evictAlg == FIFO ? "modified fifo" : evictAlg == CLOCK ? "clock" : "aging");
}
