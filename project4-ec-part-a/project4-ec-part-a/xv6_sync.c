// xv6_sync.c

#include "xv6_sync.h"
#include "x86.h"

// Custom spinlock implementation
void spinlock_init(spinlock_t *lock) {
  lock->locked = 0;
}

void spinlock_acquire(spinlock_t *lock) {
  while (xchg(&lock->locked, 1) != 0)
    ;
}

void spinlock_release(spinlock_t *lock) {
  xchg(&lock->locked, 0);
}

// Ticket lock implementation
void ticket_lock_init(ticket_lock_t *lock) {
  lock->next_ticket = 0;
  lock->now_serving = 0;
}

void ticket_lock_acquire(ticket_lock_t *lock) {
  uint my_ticket = fetch_and_add(&lock->next_ticket, 1);
  while (lock->now_serving != my_ticket)
    ;
}

void ticket_lock_release(ticket_lock_t *lock) {
  fetch_and_add(&lock->now_serving, 1);
}

// Reader-writer lock implementation
void rw_lock_init(rw_lock_t *lock) {
  lock->num_readers = 0;
  lock->num_writers_waiting = 0;
  lock->writer_active = 0;
  ticket_lock_init(&lock->writer_lock);
  spinlock_init(&lock->reader_lock);
}

void rw_lock_acquire_reader(rw_lock_t *lock) {
  spinlock_acquire(&lock->reader_lock);
  fetch_and_add(&lock->num_readers, 1);
  spinlock_release(&lock->reader_lock);
}

void rw_lock_release_reader(rw_lock_t *lock) {
  fetch_and_add(&lock->num_readers, -1);
}

void rw_lock_acquire_writer(rw_lock_t *lock) {
  fetch_and_add(&lock->num_writers_waiting, 1);
  ticket_lock_acquire(&lock->writer_lock);
  fetch_and_add(&lock->num_writers_waiting, -1);
  lock->writer_active = 1;
}

void rw_lock_release_writer(rw_lock_t *lock) {
  lock->writer_active = 0;
  ticket_lock_release(&lock->writer_lock);
}

// Queue lock implementation
void queue_lock_init(queue_lock_t *lock) {
  lock->head = 0;
  lock->tail = 0;
  lock->locked = 0;
}

void queue_lock_acquire(queue_lock_t *lock) {
  queue_node_t node;
  node.next = 0;

  queue_node_t *tail = xchg(&lock->tail, &node);
  if (tail != 0) {
    tail->next = &node;
    while (lock->locked != 0)
      ;
    lock->locked = 1;
    lock->head = node.next;
  } else {
    while (xchg(&lock->locked, 1) != 0)
      ;
  }
}

void queue_lock_release(queue_lock_t *lock) {
  if (lock->head == 0) {
    lock->locked = 0;
  } else {
    lock->locked = 0;
    lock->head = lock->head->next;
  }
}
