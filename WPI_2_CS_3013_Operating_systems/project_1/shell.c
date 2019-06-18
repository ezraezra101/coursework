// Gareth Solbeck & Ezra Davis
// Part 2 of HW 1 for CS 3013: Operating Systems

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#define TRUE	-1
#define PROMPT	"> "
#define INDENT	"- "
#define LINE	"--------\n"
#define DELIMITERS " \n"

void printProcessUsage(pid_t procNum, struct timeval startTime);
int runCommand(char **cmdTok);

static char noStats = 0;

int main(int argc, char **argv) {
	
	//Adding an option for not printing out stats.
	if(argc > 1 && strcmp(argv[1], "--no-stats")==0) {
		noStats = TRUE;
	} else if(argc > 1) {
		printf("Usage: \n\t%s [--no-stats]\n", argv[0]);
		exit(0);
	}



	char cmd[130];
	
	while(TRUE) {
		printf(PROMPT);
		fflush(stdout);
		
		if (feof(stdin)) {
			printf("[End of file]\n");
			exit(0);
		}
		
		int j;
		for (j = 0; cmd[j - 1] != '\n' && cmd[j - 1] != EOF; j++) {
			cmd[j] = fgetc(stdin);
			if (j > 128) {
				printf("Error: Command should be less than 129 characters.\n");
				//Put excess character back into buffer (in case it was newline)
				ungetc(cmd[j], stdin);

				// flush line
				char c;
				do {
					c = fgetc(stdin);
				} while (c != '\n' && c != EOF);
				break;
			}
		}
		cmd[j-1] = '\n';
		cmd[j]   = '\0';
		
		if (j > 128) {
			continue;
		}
		
		//Tokenize the input string for runCommand()
		int i = 1;
		char *tokens[33];
		tokens[0] = strtok(cmd, DELIMITERS);
		
		while(tokens[i - 1] != NULL && i <= 32) {
			tokens[i] = strtok(NULL, DELIMITERS);
			i++;
		}
		if(i > 32) {
			printf("Error: command should have less than 32 arguments.\n");
			continue;
		}
		
		runCommand(tokens);
	}
	
	return 0;
}

// Runs a command represented by a list of tokens
int runCommand(char **cmdTok) {
	// int j = 0;
	// while (cmdTok[j] != NULL) {
	// 	printf("Command: <%s>\n", cmdTok[j]);
	// 	j++;
	// }

	if(cmdTok[0] == NULL) {
		return 0;
	} else if(!strcmp(cmdTok[0], "cd")) {
		chdir(cmdTok[1]);
		return 0;
	} else if(!strcmp(cmdTok[0], "exit")) {
		exit(0);
	}
	
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
		
		if(!noStats) {
			printProcessUsage(childProcNum, startTime);
		}
		
	} else {
		//In child process
		
		//Run command, if there's an error, exit.
		if( execvp(cmdTok[0], cmdTok) == -1 ) {
			exit(errno);
		}
	}
	return 0;
}

// Prints usage statistics about a process that has just ended
void printProcessUsage(pid_t procNum, struct timeval startTime) {

	static double lastCPUTime = 0;
	static int lastInvolPreemptions = 0;
	static int lastVolPreemptions = 0;
	static int lastPageFaults = 0;
	static int lastPageReclaims = 0;
	
	struct timeval endTime;
	struct rusage usageData;
	gettimeofday(&endTime, NULL);
	getrusage(RUSAGE_CHILDREN, &usageData);
	
	printf("\n" LINE);
	
	// => Elapsed wall-clock time (milliseconds)
	double elapsedTime =
		(endTime.tv_sec-startTime.tv_sec)*1000.0f
		+ (endTime.tv_usec - startTime.tv_usec)/1000.0f;
	printf(INDENT "%8.3lf milliseconds passed.\n", elapsedTime);
	
	// => CPU time (both user & system) (milliseconds)
	double userTime = usageData.ru_utime.tv_sec * 1000
		+ usageData.ru_utime.tv_usec / 1000.0f;
	double systemTime = usageData.ru_stime.tv_sec * 1000
		+ usageData.ru_stime.tv_usec / 1000.0f;
	double cpuTime = userTime + systemTime;
	printf(INDENT "%8.3lf ms of CPU time (user and system) used\n",
		cpuTime - lastCPUTime);
	
	// => # of times process was preempted involuntarily ((time quantum expired, preemption by higher priority process, etc.),
	printf(INDENT "%8ld involuntary process preemptions\n",
		usageData.ru_nivcsw - lastInvolPreemptions);
	
	// => the number of times the process gave up the CPU voluntarily (waiting for I/O or a resource),
	printf(INDENT "%8ld voluntary process preemptions\n",
		usageData.ru_nvcsw - lastVolPreemptions);
	
	// => the number of page faults
	printf(INDENT "%8ld page faults\n", usageData.ru_majflt - lastPageFaults);
	
	// => the number of page faults that could be satisfied using unreclaimed pages.
	printf(INDENT "%8ld page faults satisfied by page reclaims\n",
		usageData.ru_minflt - lastPageReclaims);

	printf(LINE);
	
	lastCPUTime = cpuTime;
	lastInvolPreemptions = usageData.ru_nivcsw;
	lastVolPreemptions = usageData.ru_nvcsw;
	lastPageFaults = usageData.ru_majflt;
	lastPageReclaims = usageData.ru_minflt;
}