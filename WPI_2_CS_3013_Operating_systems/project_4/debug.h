// Gareth Solbeck & Ezra Davis
// HW 4 for CS 3013: Operating Systems

int debug_mutex_lock(pthread_mutex_t *lock);
int debug_mutex_unlock(pthread_mutex_t *lock);
int debug_mutex_trylock(pthread_mutex_t *lock);
void print_locked_pages();

