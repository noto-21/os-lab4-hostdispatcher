#include <stdio.h>
#include <stdlib.h>
#include "process.h"

#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct {
    /** Pointer to the process data associated with this node. */
    process_t *process;
    /** Pointer to the next node in the queue, NULL if it's the last node. */
    struct node_t *prev;
    /** Pointer to the previous node in the queue, NULL if it's the first node. */
    struct node_t *next;
} node_t;

// Alias node_t as queue_t for clarity when used to represent a queue
typedef node_t queue_t;

/**
 * Adds a new process to the end of the queue.
 * 
 * @param queue Pointer to the head of the queue.
 * @param process Pointer to the process to be added to the queue.
 * 
 * Dynamically allocates memory for a new node, assigns the process to it,
 * and inserts it at the end of the queue. Assumes that the passed 'queue' 
 * pointer points to a dummy head node to keep things simple.
 */
extern void push(queue_t *queue, process_t *process);

/**
 * Removes and returns the last process from the queue.
 * 
 * @param queue Pointer to the head of the queue.
 * @return Pointer to the removed process, or NULL if the queue is empty.
 * 
 * This function traverses the queue to find the last process, removes it from the queue,
 * and returns it. It manages the pointers of the preceding node to bypass the removed node.
 * Assumes the 'queue' pointer points to a dummy head node.
 */
extern process_t *pop(queue_t *queue);

#endif /* QUEUE_H_ */
