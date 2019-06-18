// Gareth Solbeck & Ezra Davis
// Part 1 of HW 1 for CS 3013: Operating Systems

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>


void printProcessUsage(pid_t procNum, struct timeval startTime);

int main(int argc, char **argv) {
	
	//Read in arguments
	char** subargs = (char**) alloca(argc * sizeof(char*));
	int i;
	for (i = 0; i < argc; i++) {
		subargs[i] = argv[i + 1];
	}
	subargs[argc - 1] = NULL;
	
	//Setup start time
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	
	pid_t childProcNum = fork();
	
	if(childProcNum != 0) {
		//In Parent process
		
		int exitStatus = 0;
		wait(&exitStatus);
		
		//If child had an error
		if(WIFEXITED(exitStatus) && WEXITSTATUS(exitStatus) != 0) {
			printf("Error: Unix exit code %d\n", WEXITSTATUS(exitStatus));
			return WEXITSTATUS(exitStatus);
		}
		
		printProcessUsage(childProcNum, startTime);
		
	} else {
		//In child process
		
		//Run command, if there's an error, exit.
		if( execvp(subargs[0], subargs) ==-1 ) {
			exit(errno);
		}
	}

	fflush(stdout);
	
	return 0;
}

// Prints usage statistics about a process that has just ended
void printProcessUsage(pid_t procNum, struct timeval startTime) {
	
	struct timeval endTime;
	struct rusage usageData;
	gettimeofday(&endTime, NULL);
	getrusage(RUSAGE_CHILDREN, &usageData);
	
	// => Elapsed wall-clock time (milliseconds)
	double elapsedTime =
		(endTime.tv_sec-startTime.tv_sec)*1000.0f
		+ (endTime.tv_usec - startTime.tv_usec)/1000.0f;
	printf("\n%8.3lf milliseconds passed.\n", elapsedTime);
	
	// => CPU time (both user & system) (milliseconds)
	double userTime = usageData.ru_utime.tv_sec * 1000
		+ usageData.ru_utime.tv_usec / 1000.0f;
	double systemTime = usageData.ru_stime.tv_sec * 1000
		+ usageData.ru_stime.tv_usec / 1000.0f;
	double cpuTime = userTime + systemTime;
	printf("%8.3lf ms of CPU time (user and system) used\n", cpuTime);
	
	// => # of times process was preempted involuntarily ((time quantum expired, preemption by higher priority process, etc.),
	printf("%8ld involuntary process preemptions\n", usageData.ru_nivcsw);
	
	// => the number of times the process gave up the CPU voluntarily (waiting for I/O or a resource),
	printf("%8ld voluntary process preemptions\n", usageData.ru_nvcsw);
	
	// => the number of page faults
	printf("%8ld page faults\n", usageData.ru_majflt);
	
	// => the number of page faults that could be satisfied using unreclaimed pages.
	printf("%8ld page faults satisfied by page reclaims\n", usageData.ru_minflt);

}