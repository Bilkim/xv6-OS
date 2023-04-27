// xv6_sync.h

#pragma once

#include "spinlock.h" // Built-in xv6 spinlock
#include "types.h"

// Custom spinlock
typedef struct {
  uint locked;
} spinlock_t;

void spinlock_init(spinlock_t *lock);
void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);

// Ticket lock
typedef struct {
  uint next_ticket;
  uint now_serving;
} ticket_lock_t;

void ticket_lock_init(ticket_lock_t *lock);
void ticket_lock_acquire(ticket_lock_t *lock);
void ticket_lock_release(ticket_lock_t *lock);

// Reader-writer lock
typedef struct {
  uint num_readers;
  uint num_writers_waiting;
  uint writer_active;
  ticket_lock_t writer_lock;
  spinlock_t reader_lock;
} rw_lock_t;

void rw_lock_init(rw_lock_t *lock);
void rw_lock_acquire_reader(rw_lock_t *lock);
void rw_lock_release_reader(rw_lock_t *lock);
void rw_lock_acquire_writer(rw_lock_t *lock);
void rw_lock_release_writer(rw_lock_t *lock);

// Queue lock
typedef struct queue_node {
  struct queue_node *next;
} queue_node_t;

typedef struct {
  queue_node_t *head;
  queue_node_t *tail;
  uint locked;
} queue_lock_t;

void queue_lock_init(queue_lock_t *lock);
void queue_lock_acquire(queue_lock_t *lock);
void queue_lock_release(queue_lock_t *lock);
