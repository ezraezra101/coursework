#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

#ifdef DEBUG
#define TIME_SCALE 1
#else
#define TIME_SCALE 10
#endif

#define NUM_CARS 20
#define TRUE -1
#define FALSE 0
#define NONE -1

#define NEXT(q) (((q) + 1) % 4)
#define PREV(q) (((q) + 3) % 4)

typedef enum {LEAVE = 0, RIGHT = 1, STRAIGHT = 2, LEFT = 3} direction_t;
typedef enum {NE = 0, NW = 1, SW = 2, SE = 3} quadrant_t;

char* dir_str[4] = {"LEAVE", "right", "straight", "left"};
char* quad_str[4] = {"NE", "NW", "SW", "SE"};

typedef enum {INTERSECTION_PLAN, CONVOY_PLAN, GEN_PLAN, ACTION} phases_t;

pthread_mutex_t barrier_lock; //should not be used outside barrier()

pthread_mutex_t quadrants[4];
direction_t     quad_dirs[4];

pthread_mutex_t quadrant_interested[4];
direction_t     quad_interested_dir[4];

int queues[4][20];
int queue_lengths[4] = {0,0,0,0};

pthread_mutex_t queue_lock;

pthread_cond_t cond_barrier;

int emergency = FALSE;
pthread_cond_t emergency_waiting;

#ifdef DEBUG
pthread_mutex_t debug_lock;

void debug_print() {
  printf("\t================\n");
  printf("\tInterested:\n\t\t%d|%d\n\t\t%d|%d\n",
   quad_interested_dir[NW],quad_interested_dir[NE],
   quad_interested_dir[SW],quad_interested_dir[SE]);
  printf("\tActual:\n\t\t%d|%d\n\t\t%d|%d\n",
   quad_dirs[NW],quad_dirs[NE],quad_dirs[SW],quad_dirs[SE]);
  printf("\t================\n");

}
#endif

int canJam() {
  
  int non_rights = 0;
  
  int i;
  for(i=0; i<4; i++) {
    if(quad_interested_dir[i] > 0) {
      non_rights++;
    }
  }
  return non_rights > 2;
}

int intersectionEmpty() {
  int i;
  for(i=0; i<4; i++) {
    if(quad_dirs[i] > 0) {
      return FALSE;
    }
  }
  return TRUE;
}

void barrier() {
  static int barrier_count = NUM_CARS;
  
  pthread_mutex_lock(&barrier_lock);

  
  if(--barrier_count <= 0) {
#ifdef DEBUG
    debug_print();
#endif
    
    pthread_cond_broadcast(&cond_barrier);
    barrier_count = NUM_CARS;
    pthread_mutex_unlock(&barrier_lock);

  } else {
    pthread_cond_wait(&cond_barrier, &barrier_lock);
    pthread_mutex_unlock(&barrier_lock);
  }
  
}

/* priority:
    intersection vehicles
    emergency/convoy vehicles
    earliest to latest vehicles outside
*/
void drive() {
  usleep(100000 * TIME_SCALE);
}

void enqueue(quadrant_t entry_quadrant, int id) {
  queues[entry_quadrant][queue_lengths[entry_quadrant]] = id;
  queue_lengths[entry_quadrant]++;
}

void dequeue(quadrant_t quadrant) {
  //Move all elements up in the queue (overwriting the first one)
  int i;
  for (i = 1; i < queue_lengths[quadrant]; i++) {
    queues[quadrant][i - 1] = queues[quadrant][i];
  }
  queue_lengths[quadrant]--;

}

int peek_queue(quadrant_t quadrant) {
  return queues[quadrant][0];
}

int intersection_plan(quadrant_t cur_location, direction_t direction) {
  quad_interested_dir[NEXT(cur_location)] = direction;
  
  return pthread_mutex_trylock(quadrant_interested + NEXT(cur_location));
}

int general_plan(int id, quadrant_t entry_quadrant, direction_t direction) {
  
  pthread_mutex_lock(&queue_lock);
  
  if (canJam()) {
    pthread_mutex_unlock(&queue_lock);
    return TRUE;
  }
  
  if (peek_queue(entry_quadrant) != id) {
    pthread_mutex_unlock(&queue_lock);
    return TRUE;
  }
  pthread_mutex_unlock(&queue_lock);
  
  if (pthread_mutex_trylock(quadrant_interested + entry_quadrant)) {
    return TRUE; // jammed
  } else {
    //printf("\tGen plan FALSE!\n");
    pthread_mutex_lock(&queue_lock);
    printf("Car %3d planning to enter %s quadrant\n", id, quad_str[entry_quadrant]);
    dequeue(entry_quadrant);
    quad_interested_dir[entry_quadrant] = direction;
    pthread_mutex_unlock(&queue_lock);
    return FALSE;
  }
}

void move(quadrant_t cur_location, direction_t direction,
    int entering_intersection) {
  
  quadrant_t desired = entering_intersection ?
    cur_location : NEXT(cur_location);
  
  // Claim our next location
  if (direction != LEAVE) {
    pthread_mutex_lock(quadrants + desired);
    quad_dirs[desired] = direction;
    //printf("\tLocked quadrant %d\n", desired);
    
    // Relinquish interest
    quad_interested_dir[desired] = LEAVE;
    pthread_mutex_unlock(quadrant_interested + desired);
  }
  // Drive
  drive();
  //printf("\tDrove!\n");
  
  
  // Allow others to enter previous location
  if(!entering_intersection) {
    //printf("\tUnlocking %d\n", cur_location);
    pthread_mutex_unlock(quadrants + cur_location);
    quad_dirs[cur_location] = LEAVE; // Prints prettier than NONE
  }
}

void emergency_move(quadrant_t quadrant, direction_t direction) {
  int d;
  drive();
  printf("[!] Emergency vehicle entering %s quadrant\n", quad_str[quadrant]);
  for (d = 0; d < direction - 1; d++) {
    drive();
    printf("[!] Emergency vehicle moving from %s to %s quadrant\n",
      quad_str[quadrant], quad_str[NEXT(quadrant)]);
    quadrant = NEXT(quadrant);
  }
  printf("[!] Emergency vehicle left the intersection from %s quadrant\n",
    quad_str[quadrant]);
}

void motorcade_move(quadrant_t quadrant, direction_t direction, int num) {
  int time_passed = 0;
  
  
  for(time_passed = 0; time_passed < num + direction; time_passed++) {
    int i;
    quadrant_t cur_quad = (quadrant + direction-1) % 4;
    
    for(i = direction; i >=0; i--) {
      if(time_passed >= i && time_passed < num + i) {
        drive();
        
        if(i == 0) {
          printf("[!] Motorcade car %3d entering %s quadrant\n", time_passed,
            quad_str[cur_quad]);
        } else if(i < direction) {
          
          printf("[!] Motorcade car %3d moving from %s to %s quadrant\n", time_passed - i,
            quad_str[PREV(cur_quad)], quad_str[cur_quad]);
            
        } else { //i == direction
          printf("[!] Motorcade car %3d left the intersection from %s quadrant\n",
            time_passed - i, quad_str[PREV(cur_quad)]);
        }
      } //End if
      
      cur_quad = PREV(cur_quad);
    }
  }
}

void manage_car(quadrant_t entry_quadrant, direction_t direction, int id) {
  // Location data
  quadrant_t quadrant = NONE;

  int in_intersection = FALSE;
  int jammed = TRUE;
  
  // Enter queue
  pthread_mutex_lock(&queue_lock);
  printf("Car %3d arrived in line for %s quadrant going %s\n",
    id, quad_str[entry_quadrant], dir_str[direction]);
  enqueue(entry_quadrant, id);
  pthread_mutex_unlock(&queue_lock);
  
  while (TRUE) {
    //Plan cars in intersection
    if (in_intersection) {
      if (LEAVE == direction) {
        //printf("\twill leave!\n");
        jammed = FALSE;
      } else {
        jammed = intersection_plan(quadrant, direction);
      }
    }
    
    barrier();
    
    //Plan cars outside of intersection
    if (!in_intersection && !emergency) {
      quadrant = entry_quadrant;
      jammed = general_plan(id, entry_quadrant, direction);
    }
    
    
    barrier();
    
    //Move cars according to plans
    if (!jammed) {
      move(quadrant, direction, !in_intersection);
      
      if (!in_intersection) {
        printf("Car %3d entering %s quadrant\n", id, quad_str[entry_quadrant]);
        in_intersection = TRUE;
        quadrant = entry_quadrant;
      } else if (direction == LEAVE) {
        printf("Car %3d left the intersection from %s quadrant\n", id,
          quad_str[quadrant]);
        if (emergency && intersectionEmpty()) {
          pthread_cond_signal(&emergency_waiting);
        }
        barrier();
        return;
        
      } else {
        printf("Car %3d moving from %s to %s quadrant\n", id,
          quad_str[quadrant], quad_str[NEXT(quadrant)]);
        quadrant = NEXT(quadrant);
      }
      
      direction--;
      
    } // end if(!jammed)
    
    barrier();
  }
}

void *thread_function(void *id) {
  int thread_id = (int) id;
  
  while (TRUE) {
    usleep(rand() % 10000 * TIME_SCALE);
    manage_car(rand() % 4, rand() % 3 + 1, thread_id);
    thread_id += NUM_CARS;
  }
}

void *emergency_thread_function(void *arg) {
  pthread_mutex_t thread_mutex;
  pthread_mutex_init(&thread_mutex, NULL);
  
  direction_t direction;
  quadrant_t entry_quadrant;
  int num;
  while (TRUE) {
    usleep((rand() % 1000000 + 1000000) * TIME_SCALE);
    emergency = TRUE;
    entry_quadrant = rand() % 4;
    direction = rand() % 3 + 1;
    num = (rand() % 2) ? 1 : (rand() % 8 + 2); // num is 1 half the time, else 2-10
    
    printf("[!] %s arrived in line for %s quadrant going %s\n",
      (num > 1) ? "Motorcade" : "Emergency vehicle", 
      quad_str[entry_quadrant], dir_str[direction]);
    
    pthread_cond_wait(&emergency_waiting, &thread_mutex);
    if (num > 1) {
      motorcade_move(entry_quadrant, direction, num);
    } else {
      emergency_move(entry_quadrant, direction);
    }
    emergency = FALSE;
    pthread_mutex_unlock(&thread_mutex);
  }
}

void mutex_init() {
  pthread_mutex_init(&barrier_lock, NULL);
  pthread_mutex_init(&queue_lock, NULL);
  int i;
  for (i = 0; i < 4; i++) {
    pthread_mutex_init(quadrants + i, NULL);
    pthread_mutex_init(quadrant_interested + i, NULL);
  }

#ifdef DEBUG
  pthread_mutex_init(&debug_lock, NULL);
#endif
}



void cond_init() {
  pthread_cond_init(&cond_barrier, NULL);
  pthread_cond_init(&emergency_waiting, NULL);
}

int main(int argc, char **argv) {
  mutex_init();
  cond_init();
  
  pthread_t threads[NUM_CARS + 1];
  int i;
  for (i = 0; i < NUM_CARS; i++) {
    /*
      pthread_create(*thread, *attr, start_routine, arg)
       - arg is passed to start_routine
       - attr is of type pthread_attr_t; if NULL thread is given default attributes
    */
    int status = pthread_create(&threads[i], NULL, thread_function, (void *) i);
    if (status != 0) { // if status is not 0, an error occurred!
      printf("Error in pthread_create: %d", errno);
      exit(errno);
    }
  }
  if (pthread_create(&threads[i], NULL, emergency_thread_function, (void *) i)) {
    printf("Error in pthread_create: %d", errno);
    exit(errno);
  }
  
  //Prevents ending of program early
  while(TRUE) {   
    pthread_yield();
  }
  return 1; // How did you get here?!
  
  
}