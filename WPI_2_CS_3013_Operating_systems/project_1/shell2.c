// Gareth Solbeck & Ezra Davis
// Part 3 of HW 1 for CS 3013: Operating Systems

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#include "shell2.h"

// If statistics are not printed out.
static char noStats = 0;

int main(int argc, char **argv) {
	
	//Adding an option for not printing out stats.
	if(argc > 1 && strcmp(argv[1], "--no-stats")==0) {
		noStats = TRUE;
	} else if(argc > 1) {
		printf("Usage: \n\t%s [--no-stats]\n", argv[0]);
		exit(0);
	}

	while(TRUE) {
		prompt();
		
		//See if any background tasks have completed.
		int finishedProcess;
		do {
			finishedProcess = waitAndPrint(WNOHANG, -1);		
		} while (finishedProcess != 0);
	}
	
	// will never reach here
	
	return 0;
}

// Prompts users for input, validates and tokenizes it, and runs the command.
void prompt() {
	// 130 characters: 128 + '\n' + '\0'
	char cmd[130];
	
	printf(PROMPT);
	fflush(stdout);
	
	if (feof(stdin)) {
		printf("[End of file]\n");
		shellExit(0);
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
			return;
		}
	}
	cmd[j-1] = '\n';
	cmd[j]   = '\0';
	
	//Detect ending '&' for backgrounding.
	int isBackground = cmd[strlen(cmd)-2] == '&';
	if(isBackground) {
		cmd[strlen(cmd)-2] = '\0';
	}
	
	//Tokenize the input string for runCommand()
	int i = 1;
	char *tokens[33];
	tokens[0] = strtok(cmd, DELIMITERS);
	
	while(tokens[i - 1] != NULL) {
		tokens[i] = strtok(NULL, DELIMITERS);
		
		if(i > 32) {
			printf("Error: command should have less than 32 arguments.\n");
			return;
		}
		i++;
	}
	
	runCommand(tokens, isBackground);
}

// Runs a command represented by a list of tokens
int runCommand(char **cmdTok, int isBackground) {

	if(cmdTok[0] == NULL) {
		return 0;
	} else if(!strcmp(cmdTok[0], "cd")) {
		if (isBackground) {
			printf("Error: Can't run cd as background process.\n");
			return 1;
		} else {
			chdir(cmdTok[1]);
			return 0;
		}
	} else if(!strcmp(cmdTok[0], "exit")) {
		if (isBackground) {
			printf("Error: Can't run exit as background process.\n");
			return 1;
		} else {
			shellExit(0);
			return 0;
		}
	} else if(!strcmp(cmdTok[0], "jobs")) {
		if (isBackground) {
			printf("Error: Can't run jobs as background process.\n");
			return 1;
		} else {
			listPrint();
			return 0;
		}
	}
	
	//Setup start time
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	
	pid_t childProcNum = fork();
	
	if(childProcNum != 0) {
		//In Parent process
		
		int exitStatus = 0;
		listAdd(childProcNum, cmdTok, startTime);
		
		int finishedProcess = 0;
		
		if(!isBackground) {
			while (finishedProcess != childProcNum) {
				finishedProcess = waitAndPrint(WNOHANG, childProcNum);		
			}			
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

// Checks for child process's completion,
//  optionally waits until a child completes.
//  Returns the completed process's ID (or 0)
//  foregroundPid is used to determine whether to print out pid's info
int waitAndPrint(int options, int foregroundPid) {
	int exitStatus;
	struct timeval startTime;
	
	pid_t pid = wait3(&exitStatus, options, NULL);
	
	// If WNOHANG specified and there are no completed child processes,
	//  wait3 will return NULL
	if(pid == -1 || pid == 0) {
		return 0;
	}
	
	if(!noStats) {
		printf("\n" LINE);
	}
	
	listRemove(pid, &startTime, !noStats || pid != foregroundPid);
	
	//If child had an error
	if(WIFEXITED(exitStatus) && WEXITSTATUS(exitStatus) != 0) {
		printf("Error: Unix exit code %d\n", WEXITSTATUS(exitStatus));
		
		if(!noStats) {
			printf(LINE);
		}
		
	} else if(!noStats) {
		printProcessUsage(pid, startTime);
	}
	
	return pid;
}

// Prints usage statistics about a process that has just ended
void printProcessUsage(pid_t procNum, struct timeval startTime) {

	fflush(stdout);

	static double lastCPUTime = 0;
	static int lastInvolPreemptions = 0;
	static int lastVolPreemptions = 0;
	static int lastPageFaults = 0;
	static int lastPageReclaims = 0;
	
	struct timeval endTime;
	struct rusage usageData;
	gettimeofday(&endTime, NULL);
	getrusage(RUSAGE_CHILDREN, &usageData);
	
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


// Exits the shell after waiting for child processes to finish.
void shellExit(int exitCode) {
	if(isListEmpty()) {
		exit(exitCode);
	} else {
		printf("Waiting for the following processes to finish.\n");
		listPrint();
		while (!isListEmpty()) {
			waitAndPrint(0, -1);
		}
		exit(exitCode);
	}
}