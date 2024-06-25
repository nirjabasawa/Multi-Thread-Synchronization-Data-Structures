//
// Nirja Basawa
// nbasawa
// UCSC CSE130
// Assignment 3
//

#include "queue.h"

# include <pthread.h>
# include <semaphore.h>
# include <stdlib.h>
#include <assert.h>

// define queue struct
struct queue {
    int size;
    int in;
    int out;
    void **buffer;
    sem_t full;
    sem_t empty;
    sem_t mutex;
};


// queue_new()
// allocates and initializes new queue with max size "size"
queue_t *queue_new(int size) {
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    // check not null
    if (q) {
        q->size = size;
        q->in = 0;
        q->out = 0;
        q->buffer = (void **) malloc(size * sizeof(void *));

        if (!q->buffer) {
            // memory alloc unsuccessful
            free(q);
            q = NULL;
        } else {
            // initialize semaphores
            int rc = 0;
            rc = sem_init(&(q->full), 0, size);
            assert(!rc);
            rc = sem_init(&(q->empty), 0, size);
            assert(!rc);
            rc = sem_init(&(q->mutex), 0, size);
            assert(!rc);
        }
    }
    // return created queue
    return q;
}

// queue_delete()
// delete queue and free memory
void queue_delete(queue_t **q) {
    // check not null
    if (*q && (*q)->buffer) {
        // destroy semaphores
        int rc = 0;
        rc = sem_destroy(&(*q)->full);
        assert(!rc);
        rc = sem_destroy(&(*q)->empty);
        assert(!rc);
        rc = sem_destroy(&(*q)->mutex);
        assert(!rc);
        // free buffer
        free((*q)->buffer);
        (*q)->buffer = NULL;
        // free q
        free(*q);
        *q = NULL;
    }
}

// queue_push()
// push elem into queue
// function should succeed unless q is NULL
bool queue_push(queue_t *q, void *elem) {
    if (q != NULL) {
        sem_wait(&(q->full));
        sem_wait(&(q->mutex));

        q->buffer[q->in] = elem;
        q->in = (q->in + 1) % (q->size);

        sem_post(&(q->mutex));
        sem_post(&(q->empty));
        return true;
    }
    return false;
}

// queue_pop()
// pop elem from queue
// function should suceed unless q is NULL
bool queue_pop(queue_t *q, void **elem) {
    if (q != NULL) {
        sem_wait(&(q->empty));
        sem_wait(&(q->mutex));

        *elem = q->buffer[q->out];
        q->out = (q->out + 1) % (q->size);

        sem_post(&(q->mutex));
        sem_post(&(q->full));
        return true;
    }
    return false;
}