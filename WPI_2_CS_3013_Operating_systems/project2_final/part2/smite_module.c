#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/syscalls.h>

#include <asm/current.h>
#include <asm/uaccess.h>

unsigned long **sys_call_table;

asmlinkage long (*ref_sys_cs3013_syscall2)(void);
asmlinkage long (*ref_sys_cs3013_syscall3)(void);

struct task_struct* get_next(struct task_struct *process, unsigned short uid);

asmlinkage long smite(unsigned short *target_uid, int *num_pids_smited,
    int *smited_pids, long *pid_states) {
  
  unsigned short ktarget_uid;
  int knum_pids_smited = 0;
  int ksmited_pids[100];
  long kpid_states[100];
  
  struct task_struct *p = NULL;
  
  int user = current_uid().val;
  if (user >= 1000) {
    printk(KERN_INFO "User %d attempted to smite without sudo privileges.\n", user);
    return 1;
  }
  
  copy_from_user(&ktarget_uid, target_uid, sizeof(unsigned short));
  
  //Move 'next' pointer to init process
  for_each_process(p) {
    
    if (p->cred->uid.val == ktarget_uid && p->state == 0) {
      kpid_states[knum_pids_smited] = p->state;
      set_task_state(p, TASK_UNINTERRUPTIBLE);
      ksmited_pids[knum_pids_smited] = p->pid;
      
      printk(KERN_INFO "User %d smote process %d (%s).\n", user, p->pid, p->comm);
    
      if (++knum_pids_smited >= 100) {
        break;
      }
    }
  }
  
  copy_to_user(num_pids_smited, &knum_pids_smited, sizeof(int));
  copy_to_user(smited_pids, ksmited_pids, 100 * sizeof(int));
  copy_to_user(pid_states, kpid_states, 100 * sizeof(long));
  
  return 0;
}


asmlinkage long unsmite(int *num_pids_smited, int *smited_pids, long *pid_states) {
  
  int knum_pids_smited;
  int ksmited_pids[100];
  long kpid_states[100];
  struct task_struct *p;
  int i, knum_pids_unsmited = 0;
  
  int user = current_uid().val;
  
  if (user >= 1000) {
    printk(KERN_INFO "User %d attempted to unsmite without sudo privileges.\n", user);
    return 1;
  }
  
  copy_from_user(&knum_pids_smited, num_pids_smited, sizeof(int));
  copy_from_user(ksmited_pids, smited_pids, 100 * sizeof(int));
  copy_from_user(kpid_states, pid_states, 100 * sizeof(long));
  
  for_each_process(p) {
    for (i = 0; i < knum_pids_smited; i++) {
      if(p->pid == ksmited_pids[i]) {
        printk(KERN_INFO "User %d unsmote process %d (%s).\n", user, p->pid, p->comm);
        wake_up_process(p);
        set_task_state(p, kpid_states[i]);
        ksmited_pids[i] = -1; //avoid a second match
        
        knum_pids_unsmited++;
        
        if(knum_pids_unsmited >= knum_pids_smited) {
          break;
        }
      }
    }
  }
  
  return 0;
}

static unsigned long **find_sys_call_table(void) {
  unsigned long int offset = PAGE_OFFSET;
  unsigned long **sct;

  while (offset < ULLONG_MAX) {
    sct = (unsigned long **) offset;
    if (sct[__NR_close] == (unsigned long *) sys_close) {
      printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%021X\n",
              (unsigned int) sct);
      return sct;
    }

    offset += sizeof(void *);
  }
    
  return NULL;
}

static void disable_page_protection(void) {
  write_cr0 (read_cr0() & (~ 0x10000));
}

static void enable_page_protection(void) {
  write_cr0 (read_cr0() | 0x10000);
}

static int __init interceptor_start(void) {
  if (!(sys_call_table = find_sys_call_table())) {
    return -1;
  }

  ref_sys_cs3013_syscall2 = (void *) sys_call_table[__NR_cs3013_syscall2];
  ref_sys_cs3013_syscall3 = (void *) sys_call_table[__NR_cs3013_syscall3];
  disable_page_protection();
  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *) smite;
  sys_call_table[__NR_cs3013_syscall3] = (unsigned long *) unsmite;
  enable_page_protection();
  printk(KERN_INFO "Loaded interceptor!\n");
  return 0;
}

static void __exit interceptor_end(void) {
  if (!sys_call_table) {
    return;
  }

  disable_page_protection();
  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *) ref_sys_cs3013_syscall2;
  sys_call_table[__NR_cs3013_syscall3] = (unsigned long *) ref_sys_cs3013_syscall3;
  enable_page_protection();
  
  printk(KERN_INFO "Unloaded interceptor!\n");
}

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);
