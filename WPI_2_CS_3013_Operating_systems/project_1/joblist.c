// Gareth Solbeck & Ezra Davis
// Part 3 of HW 1 for CS 3013: Operating Systems

//Functions pertaining to the storage of Job information for shell2.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct job {
	int pid;
	int jid; // Job ID
	char *cmd;
	struct timeval startTime;
	struct job *next;

} Job;


static Job *listFront;

// Adds a job to our job list
int listAdd(int pid, char **cmdTok, struct timeval startTime) {
	static int jobCount = 1;
	
	// Creating new job
	Job *newJob = malloc(sizeof(Job));
	

	int j;
	int cmdLen = 0;
	for(j=0; cmdTok[j] != NULL; j++) {
		cmdLen += strlen(cmdTok[j]) + 1;
	}

	char *jobCmd = malloc(sizeof(char) * cmdLen);
	*jobCmd = '\0';
	
	int i;
	for(i=0;i<j;i++) {
		strcat(jobCmd, cmdTok[i]);
		strcat(jobCmd, " ");
	}
	
	newJob->pid = pid;
	newJob->jid = jobCount++;
	newJob->cmd = jobCmd;
	newJob->startTime = startTime;
	newJob->next= NULL;
	
	
	Job **jobPtr = &listFront;
	// jobPtr is a Job** to allow the reference ("next") to be modified.
	
	while (*jobPtr != NULL) {
		// Moving jobPtr to next job.
		jobPtr = &((*jobPtr)->next);
	}
	
	*jobPtr = newJob;
	
	return 0;
}

// Removes a job from the job list
int listRemove(int pid, struct timeval *startTime, int isVerbose) {
	Job *curJob = listFront;
	Job **refToCurJob = &listFront;
	
	// Loop while pid not found and not at the end of the list
	while(curJob != NULL && curJob->pid != pid) {
		refToCurJob = &curJob->next;
		curJob = curJob->next;
	}
	
	// If pid not found, return 1
	if(curJob == NULL) {
		return 1;
	// Else remove the job from the list and delete it
	} else {
		if(isVerbose) {
			printf("Job finished: [%d]\t%d\t%s\n", (*refToCurJob)->jid, (*refToCurJob)->pid, (*refToCurJob)->cmd);
		}
		
		*refToCurJob = curJob->next;
		*startTime = curJob->startTime;
		free(curJob->cmd);
		free(curJob);
		
		return 0;
	}
}

// Prints out a list of jobs
void listPrint() {
	if(listFront == NULL) {
		printf("No jobs to print.\n");
		return;
	}
	
	Job *toPrint = listFront;
	
	printf("\nNum\tPID\tCMD\n");
	
	while(toPrint != NULL) {
		printf("[%d]\t%d\t%s\n", toPrint->jid, toPrint->pid, toPrint->cmd);
		toPrint=toPrint->next;
	}
	return;
}

int isListEmpty() {
	return listFront == NULL;
}
