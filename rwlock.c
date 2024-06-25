//
// Nirja Basawa
// nbasawa
// UCSC CSE130
// Assignment 3
//

#include "rwlock.h"
#include <pthread.h>
#include <stdlib.h>
//#include <stdbool.h>

// helper funcs declaration
//bool writeShouldWait(rwlock_t *rw);
//bool readShouldWait(rwlock_t *rw);


// define rwlock struct
struct rwlock {
    pthread_mutex_t mutex;
    pthread_cond_t readers_cond;
    pthread_cond_t writers_cond;
    int readers;
    int writers;
    int waiting_writers;
    //int waiting_readers;
    PRIORITY priority;
    uint32_t n;
};

// rwlock_new()
// initialize with specified priority and n val for n-way priority
rwlock_t* rwlock_new(PRIORITY p, uint32_t n) {
    rwlock_t *rwlock = (rwlock_t*) malloc(sizeof(rwlock_t));
    // check not null
    if (!rwlock) {
        return NULL;
    }
    pthread_mutex_init(&rwlock->mutex, NULL);
    pthread_cond_init(&rwlock->readers_cond, NULL);
    pthread_cond_init(&rwlock->writers_cond, NULL);
    rwlock->readers = 0;
    rwlock->writers = 0;
    rwlock->waiting_writers = 0;
    //rwlock->waiting_readers = 0;
    rwlock->priority = p;
    rwlock->n = n;

    return rwlock;
}

// rwlock_delete()
// frees memory, delete rwlock l
void rwlock_delete(rwlock_t **l) {
    // check not null
    if (l == NULL || *l == NULL) {
        return;
    }
    pthread_mutex_destroy(&(*l)->mutex);
    pthread_cond_destroy(&(*l)->readers_cond);
    pthread_cond_destroy(&(*l)->writers_cond);
    free(*l);
    *l = NULL;
}

// reader_lock()
// locks shared resource, allow multiple reader access but no writers
void reader_lock(rwlock_t *rw) {
    // lock mutex
    pthread_mutex_lock(&rw->mutex);
    //rw->waiting_readers++;

    // change to readWaits
    /*while (readShouldWait(rw)) {
        pthread_cond_wait(&rw->readers_cond, &rw->mutex);
    }*/
    if (rw->priority == READERS || (rw->priority == N_WAY && rw->writers == 0)) {
        while (rw->waiting_writers > 0) { // comment waiting out?
            pthread_cond_wait(&rw->readers_cond, &rw->mutex);
        }
    }

    //rw->waiting_readers--;
    rw->readers++;
    // unlock mutex
    pthread_mutex_unlock(&rw->mutex);
}

// reader_unlock()
// reader lock is released, allows writer to acquire lock
void reader_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->readers--;
    if (rw->readers == 0 && rw->waiting_writers > 0) {
        pthread_cond_signal(&rw->writers_cond);
    }
    pthread_mutex_unlock(&rw->mutex);
}

// writer_lock()
// locks shared resource for writing, allows single writer access at a time
void writer_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_writers++;

    /*while (writeShouldWait(rw)) {
        pthread_cond_wait(&rw->writers_cond, &rw->mutex);
    }*/
    if (rw->priority == WRITERS
        || (rw->priority == N_WAY && rw->readers == 0 && rw->writers == 0)) {
        while (rw->readers > 0 || rw->writers > 0) {
            pthread_cond_wait(&rw->writers_cond, &rw->mutex);
        }
    }

    rw->waiting_writers--;
    rw->writers++;
    pthread_mutex_unlock(&rw->mutex);
}

// writer_unlock()
// unlocks shared resource for current writer, allows reader(s) to acquire lock
void writer_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->writers--;
    // check priority
    if (rw->priority == N_WAY) {
        pthread_cond_broadcast(&rw->readers_cond);
    } else {
        pthread_cond_signal(&rw->writers_cond);
    }
    pthread_mutex_unlock(&rw->mutex);
}

/*
// writeWaits()
// write should wait if active read or active write
// if N_WAY - write should wait until n reads
bool writeShouldWait(rwlock_t *rw) {
    //if (rw->priority)
    // For any priority, there shouldn't be any active reader/writer
    if (rw->writers > 0 || rw->readers > 0) {
        return true;
    }
    // for priority WRITERS - if nothing is holding the lock then no wait

    // for priority READERS
    if (rw->priority == READERS) {
        // if there are waiting readers, write should wait
        return (rw->waiting_readers > 0);
    }

    // For priority N_WAY
    if (rw->priority == N_WAY) {
        if ((rw->waiting_readers >= (int) rw->n) && rw->writers == 0 && rw->readers == 0) {
            return true;
        }
    }
    return false;
}

// readWaits()
// read should wait if active / waiting writers
bool readShouldWait(rwlock_t *rw) {
    if (rw->priority == READERS) {
        return (rw->writers > 0);
    }
    if (rw->priority == WRITERS) {
        return (rw->writers > 0 || rw->waiting_writers > 0);
    }

    if (rw->priority == N_WAY) {
        //Active writer, wait for reading
        if (rw->writers > 0) {
            return true;
        }

        //Active readers = n , wait to read
        if (rw->waiting_writers > 0 && rw->readers >= (int) rw->n) {
            return true;
        }
    }
    return false;
}

*/