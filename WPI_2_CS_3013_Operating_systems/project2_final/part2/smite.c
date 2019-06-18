// Gareth Solbeck & Ezra Davis
// Part 2 of HW 2 for CS 3013: Operating Systems

// Uses the smite call to mark processes owned by a specified user as
//  unrunnable. The stopped processes are printed out (in a format compatible
//  with unsmite input).

#include <sys/syscall.h>
#include <stdio.h>


// These values MUST match the unistd_32.h modifications
#define __NR_cs3013_syscall1 355
#define __NR_cs3013_syscall2 356
#define __NR_cs3013_syscall3 357

unsigned short target_uid;
int num_pids_smited;
int smited_pids[100];
long pid_states[100];

long smite (void) {
  return (long) syscall(__NR_cs3013_syscall2, &target_uid, &num_pids_smited,
      smited_pids, pid_states);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Invalid number of arguments to smite.\n"
      "Usage: smite [userid]\n");
    return 1;
  }
  
  target_uid = atoi(argv[1]);
  int result = smite();
  
  if (result == 1) { // smite was run without sudo privileges
    printf("User must be root to smite.\nThis incident will be reported.\n");
    return 1;
  } else if (result != 0) { // some other error occurred
    return result;
  }
  
  int i;
  for (i = 0; i < num_pids_smited; i++) {
    printf("%d %ld\n", smited_pids[i], pid_states[i]);
  }
  return 0;
}
