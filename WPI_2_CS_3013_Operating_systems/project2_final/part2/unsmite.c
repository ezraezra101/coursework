// Gareth Solbeck & Ezra Davis
// Part 2 of HW 2 for CS 3013: Operating Systems

// Uses the system call unsmite to restore processes stopped by smite
//  to their (specified) original state.

#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>


// These values MUST match the unistd_32.h modifications
#define __NR_cs3013_syscall1 355
#define __NR_cs3013_syscall2 356
#define __NR_cs3013_syscall3 357

int num_pids_smited;
int smited_pids[100];
long pid_states[100];

long unsmite (void) {
  return (long) syscall(__NR_cs3013_syscall3, &num_pids_smited,
      smited_pids, pid_states);
}

int main(int argc, char** argv) {
  printf("For each process you want to unsmite, enter its pid and previous state,\n"
    "followed by a newline. Enter only one pid and state per line. Indicate the\n"
    "end of your list by entering CTRL-D on a new line.\n"
    "(Note: output from smite may be used directly as input to unsmite.)\n");
  int i;
  char* line = malloc(sizeof(char) * 128);
  int size = 128;
  for (i = 0; i < 100; i++) {
    getline(&line, &size, stdin);
    if (feof(stdin)) {
      break;
    } else if (sscanf(line, "%d %ld\n", &smited_pids[i], &pid_states[i]) != 2) {
      printf("Please enter the pid and state as decimal integers, separated by a space.\n");
      i--;
    }
  }
  num_pids_smited = i;
  
  int result = unsmite();
  
  if (result == 1) { // unsmite was run without sudo privileges
    printf("User must be root to unsmite.\nThis incident will be reported.\n");
    return 1;
  }
  
  return result;
}
