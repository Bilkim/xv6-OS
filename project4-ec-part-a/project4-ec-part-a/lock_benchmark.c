#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "thread.h"
#include "xv6_sync.h" // Header file with the custom lock implementations

#define N_THREADS 4
#define N_ITERATIONS 10000

void test_spinlock(void*);
void test_ticket_lock(void*);
void test_rw_lock(void*);
void test_queue_lock(void*);

int main(void) {
  printf(1, "Starting lock benchmark\n");

  printf(1, "Built-in xv6 spinlock\n");
  test_spinlock(0);

  printf(1, "Your spinlock implementation\n");
  test_spinlock(1);

  printf(1, "Your ticket lock implementation\n");
  test_ticket_lock(0);

  printf(1, "Your reader-writer lock implementation\n");
  test_rw_lock(0);

  printf(1, "Your queue lock implementation\n");
  test_queue_lock(0);

  printf(1, "Lock benchmark finished\n");
  exit();
}

void test_spinlock(void *arg) {
  int is_custom = (int)arg;
  spinlock_t lock;

  if (is_custom) {
    spinlock_init(&lock);
  } else {
    initlock(&lock, "xv6 spinlock");
  }

  int i;
  for (i = 0; i < N_THREADS; i++) {
    thread_create(is_custom ? spinlock_acquire : acquire, &lock);
  }

  for (i = 0; i < N_THREADS; i++) {
    thread_join();
  }
}

void test_ticket_lock(void *arg) {
  ticket_lock_t lock;
  ticket_lock_init(&lock);

  int i;
  for (i = 0; i < N_THREADS; i++) {
    thread_create(ticket_lock_acquire, &lock);
  }

  for (i = 0; i < N_THREADS; i++) {
    thread_join();
  }
}


void test_rw_lock(void *arg) {
  rw_lock_t lock;
  rw_lock_init(&lock);

  int i;
  for (i = 0; i < N_THREADS; i++) {
    thread_create(rw_lock_acquire_reader, &lock);
  }

  for (i = 0; i < N_THREADS; i++) {
    thread_join();
  }
}


void test_queue_lock(void *arg) {
  queue_lock_t lock;
  queue_lock_init(&lock);

  int i;
  for (i = 0; i < N_THREADS; i++) {
    thread_create(queue_lock_acquire, &lock);
  }

  for (i = 0; i < N_THREADS; i++) {
    thread_join();
  }
}

