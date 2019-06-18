#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 20

#define TRUE 1

//#define DEVELOPMENT

#ifdef DEVELOPMENT
#define TIME_MULTIPLICATION_CONST 1
#else
#define TIME_MULTIPLICATION_CONST 1000
#endif

// Type of job being run,
// Unclassified, Secret, or Top Secret
typedef enum {NONE=0, UNCLASSIFIED=1, SECRET=2, TOP_SECRET=3} classified_t;

char *classificationStrings[] = {
	"none",
	"unclassified",
	"secret",
	"top secret"
};

#define CLASSIFIED_TO_S(s) ((s) > 3 || (s)<0 ? "ILLEGAL" : classificationStrings[(s)])

#define NEXT_CLASSIFICATION(s, m) ((s) == TOP_SECRET ?\
	((m)=!(m))+1 : ((s)+1)%3+1)

//Don't call these outside a critical region
void add_to_queue(int job_id, classified_t job_type);
int is_my_turn(int job_id);
void remove_from_queue();
int is_cluster_legal(classified_t job_type);
int add_to_cluster(classified_t job_type);
void remove_from_cluster(int cluster_half);
////////////////////////////////////////////


int schedule_job(int job_id, classified_t job_type);
void run_job(int job_id, classified_t job_type, int cluster_half);
void complete_job(int cluster_half);
inline void check_cluster_integrity();

classified_t get_sec_level(int thread_index);

void *thread_function(void *id);

void semaphore_init();





// Global variables
sem_t top_secrets_waiting; // Unused :-(
sem_t open_slots;

sem_t scheduling_lock;

classified_t jobs_running[2] = { NONE, NONE };

int queues[4][N];
int queue_lengths[4];

// Number of jobs to run of the current type before changing.
static int turn_timeout = 5;


///////////////////////////////////////////
///     Functions that shouldn't be     ///
/// called outside of a critical region ///
///////////////////////////////////////////

void add_to_queue(int job_id, classified_t job_type) {
	queues[job_type][queue_lengths[job_type]] = job_id;
	queue_lengths[job_type]++;
}

classified_t next_turn_type() {
	static classified_t queue_turn = UNCLASSIFIED;
	
	if(queue_lengths[TOP_SECRET] > 2) {
		queue_turn = TOP_SECRET;
		turn_timeout = 2;
	}
	
	while(queue_lengths[queue_turn] == 0 || turn_timeout == 0) {
		
		if(queue_lengths[SECRET] == 0 && queue_lengths[UNCLASSIFIED] == 0) {
			queue_turn = TOP_SECRET;
		} else {
			static int secret_next = 0;
			queue_turn = NEXT_CLASSIFICATION(queue_turn, secret_next);
		}
		turn_timeout = 5;
	}
	
	return queue_turn;
}

int is_my_turn(int job_id) {
	return job_id == queues[next_turn_type()][0];
}

//Removes the next item from the queue.
void remove_from_queue() {
	classified_t job_type = next_turn_type();
	
	//Move all elements up in the queue (overwriting the first one)
	int i;
	for (i = 1; i < queue_lengths[job_type]; i++) {
		queues[job_type][i - 1] = queues[job_type][i];
	}
	
	queue_lengths[job_type]--;
	turn_timeout--;
}

// Can the job go into the cluster without compromising security?
int is_cluster_legal(classified_t job_type) {
	if(job_type >= SECRET) {
		return !(jobs_running[0] == UNCLASSIFIED
			|| jobs_running[1] == UNCLASSIFIED);
	} else {
		return !(jobs_running[0] >= SECRET
			|| jobs_running[1] >= SECRET);
	}
}

// Places a job in half of the cluster. Returns which half.
inline int add_to_cluster(classified_t job_type) {
	if(NONE == jobs_running[0]) {
		jobs_running[0] = job_type;
		return 0;
	} else if(NONE == jobs_running[1]) {
		jobs_running[1] = job_type;
		return 1;
	} else {
		printf("Tried to add job to full cluster.\n");
		exit(1);
	}
}

// Removes a job from the cluster. Does no error checking.
inline void remove_from_cluster(int cluster_half) {
	jobs_running[cluster_half] = NONE;
}
//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////



// Schedules the job or blocks until it is able to do so.
// Returns which cluster it is scheduled on.
int schedule_job(int job_id, classified_t job_type) {
	
	sem_wait(&scheduling_lock);
	add_to_queue(job_id, job_type);
	sem_post(&scheduling_lock);
	
	do {
		
		sem_wait(&open_slots);
		sem_wait(&scheduling_lock);
		
		//Is it this job's turn? and can it go into the cluster?
		if(is_my_turn(job_id) && is_cluster_legal(job_type)) {
			
			remove_from_queue();
			
			int cluster_half = add_to_cluster(job_type);
			sem_post(&scheduling_lock);
			return cluster_half;
		}
		
		sem_post(&open_slots);
		sem_post(&scheduling_lock);
		pthread_yield();
		
	} while(TRUE);
}

void run_job(int job_id, classified_t job_type, int cluster_half) {
	printf("\tStarting %s job %d in the %s half of the cluster.\n",
		CLASSIFIED_TO_S(job_type), job_id, cluster_half ? "left":"right");
	
	//Run between 0.25 and 2 seconds.
	usleep(TIME_MULTIPLICATION_CONST * (250 +  rand()%1750));
	
	printf("\tFinished %s job %d in the %s half of the cluster.\n",
		CLASSIFIED_TO_S(job_type), job_id, cluster_half ? "left":"right");
	
	return;
}

void complete_job(int cluster_half) {
	sem_wait(&scheduling_lock);
	remove_from_cluster(cluster_half);
	sem_post(&scheduling_lock);
	sem_post(&open_slots);
}

//Returns the security level of the thread in question
classified_t get_sec_level(int thread_index) {
	switch(abs(thread_index) % 10) {
		case 0:
		case 1:
		case 2:
		case 3:
			return 1;
		case 4:
		case 5:
		case 6:
			return 2;
		case 7:
		case 8:
		case 9:
			return 3;
		default:
			printf("Modulo broke...");
			exit(1);
	}
}

inline void check_cluster_integrity() {
	sem_wait(&scheduling_lock);
	
	if(jobs_running[0] == NONE || jobs_running[1] == NONE) {
		//Everything is fine
	} else if((jobs_running[0] >= SECRET)
			!= (jobs_running[1] >= SECRET)) {
		printf("Illegal cluster state! %s in cluster with %s!\n",
		 CLASSIFIED_TO_S(jobs_running[0]),
		 CLASSIFIED_TO_S(jobs_running[1]));
		exit(1);
	}
	
	sem_post(&scheduling_lock);
}

/*
  This is our start routine
  It must accept a single argument of type void*
*/
void *thread_function(void *id) {
	classified_t sec_level = get_sec_level((int) id);
	int job_id = (int) id; // ID of the current job
	
	while(TRUE) {
			
			usleep(TIME_MULTIPLICATION_CONST * (rand()%10000));

			int cluster_half = schedule_job(job_id, sec_level);			
			check_cluster_integrity();
			run_job(job_id, sec_level, cluster_half);
			complete_job(cluster_half);
			
			job_id += N;
	}

  pthread_exit(0); // How did you get here?!
}


// Initializes our semaphores.
void semaphore_init() {
	int error = 0;
	if (sem_init(&top_secrets_waiting, 0, 3)) {
		error = errno;
	} else if (sem_init(&open_slots, 0, 2)) {
		error = errno;
	} else if (sem_init(&scheduling_lock, 0, 1)) {
		error = errno;
	}
	
	if (error) {
		printf("Error in semaphore_init: %d", error);
		exit(error);
	}
	return;
}


int main(void) {
  semaphore_init();
	
  pthread_t threads[N];
  int i;
  for (i = 0; i < N; i++) {
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
	
	//Prevents ending of program early
	while(TRUE) {
		pthread_yield();
	}
	return 1; // How did you get here?!
}

