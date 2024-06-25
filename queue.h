// 
// Nirja Basawa
// 05/21/24
// queue.h

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

// create queue struct type
typedef struct queue queue_t;

// new queue - dynamically allocate and initialize queue with max size 'size'
queue_t *queue_new(int size);

// delete queue and free memory
void queue_delete(queue_t **q);

// push element elem onto queue q, return boolean value indicating success or failure
bool queue_push(queue_t *q, void *elem);

// pop element elem from queue q, return boolean value indicating success or failure
bool queue_pop(queue_t *q, void **elem);
