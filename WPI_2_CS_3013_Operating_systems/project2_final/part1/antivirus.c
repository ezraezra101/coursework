#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/string.h>

#define MIN_ID 1000

unsigned long **sys_call_table;

asmlinkage long (*ref_open)(const char __user *filename, int flags, umode_t mode);
asmlinkage long (*ref_close)(unsigned int fd);
asmlinkage long (*ref_read)(unsigned int fd, char __user *buf, size_t count);

asmlinkage long new_open(const char __user *filename, int flags, umode_t mode) {
  uid_t user = current_uid().val;
  if (user >= 1000) {
    printk(KERN_INFO "User %d is opening file:\t%s\n", user, filename);
  }
  return ref_open(filename, flags, mode);
}

asmlinkage long new_close(unsigned int fd) {
  uid_t user = current_uid().val;
  if (user >= 1000) {
    printk(KERN_INFO "User %d is closing file descriptor:\t%d\n", user, fd);
  }
  return ref_close(fd);
}

asmlinkage long new_read(unsigned int fd, char __user *buf, size_t count) {
  uid_t user = current_uid().val;
  long result = ref_read(fd, buf, count);
  //If not root
  if (user >= MIN_ID && strstr(buf, "virus") != NULL) {
    printk(KERN_INFO "User %d read from file descriptor %d, "
        "but that read contained a VIRUS!\n", user, fd);
  }
  return result;
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
  
  ref_open = (void *) sys_call_table[__NR_open];
  ref_close = (void *) sys_call_table[__NR_close];
  ref_read = (void *) sys_call_table[__NR_read];
  disable_page_protection();
  sys_call_table[__NR_open] = (unsigned long *) new_open;
  sys_call_table[__NR_close] = (unsigned long *) new_close;
  sys_call_table[__NR_read] = (unsigned long *) new_read;
  enable_page_protection();
  printk(KERN_INFO "Loaded interceptor!\n");
  return 0;
}

static void __exit interceptor_end(void) {
  if (!sys_call_table) {
    return;
  }
  
  disable_page_protection();
  // printk(KERN_INFO "page protection disabled\n");
  sys_call_table[__NR_open] = (unsigned long *) ref_open;
  // printk(KERN_INFO "ref_open returned\n");
  sys_call_table[__NR_close] = (unsigned long *) ref_close;
  // printk(KERN_INFO "ref_close returned\n");
  sys_call_table[__NR_read] = (unsigned long *) ref_read;
  // printk(KERN_INFO "ref_read returned\n");
  enable_page_protection();
  
  printk(KERN_INFO "Unloaded interceptor!\n");
}

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);
