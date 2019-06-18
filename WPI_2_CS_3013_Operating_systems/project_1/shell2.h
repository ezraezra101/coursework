// Gareth Solbeck & Ezra Davis
// Part 3 of HW 1 for CS 3013: Operating Systems

//Basic #defines used in shell2.c:
#define TRUE	-1
#define FALSE	0
#define PROMPT	"> "
#define INDENT	"- "
#define LINE	"--------\n"
#define DELIMITERS " \n"

//////////////////////////
//Functions in shell2.c //
//////////////////////////

// Prompts users for input, validates and tokenizes it, and runs the command.
void prompt();

// Runs a command represented by a list of tokens
int runCommand(char **cmdTok, int isBackground);

// Checks for child process's completion,
//  optionally waits until a child completes.
//  Returns the completed process's ID (or 0)
//  foregroundPid is used to determine whether to print out pid's info
int waitAndPrint(int options, int foregroundPid);

// Prints usage statistics about a process that has just ended
void printProcessUsage(pid_t procNum, struct timeval startTime);

// Exits the shell after waiting for child processes to finish.
void shellExit(int exitCode);



////////////////////////////
//Functions in joblist.c: //
////////////////////////////

// Adds a job to our job list
int listAdd(int pid, char **cmdTok, struct timeval startTime);

// Removes a job from the job list
int listRemove(int pid, struct timeval *startTime, int isVerbose);

// Prints out a list of jobs
void listPrint();

int isListEmpty();