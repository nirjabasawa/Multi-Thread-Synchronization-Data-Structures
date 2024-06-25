//
// Nirja Basawa
// 05/21/24
// rwlock.h
//

#pragma once

#include <stdint.h>

// create rwlock type struct
typedef struct rwlock rwlock_t;

// define levels of priority
typedef enum { READERS, WRITERS, N_WAY } PRIORITY;

// create new rwlock, dynamically allocate memory with PRIORITY p and n for N-WAY priority if needed
rwlock_t *rwlock_new(PRIORITY p, uint32_t n);

// delete rwlock and free memory
void rwlock_delete(rwlock_t **rw);

// check and allow the thread to acquire the rwlock rw for reading
void reader_lock(rwlock_t *rw);

// allow the thread to release rwlock rw for reading
void reader_unlock(rwlock_t *rw);

// check and allow the thread to acquire the rwlock rw for writing
void writer_lock(rwlock_t *rw);

// allow the thread to release rwlock rw for writing
void writer_unlock(rwlock_t *rw);
