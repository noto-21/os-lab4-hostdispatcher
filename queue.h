#include <stdio.h>
#include <stdlib.h>
#include "process.h"

#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct node_t {
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
 * Creates a new queue.
 * 
 * @param queue Pointer to the head of the queue.
 * 
 * Dynamically allocates memory for a new node and assigns it to the queue pointer.
 * Assumes that the passed 'queue' pointer points to a dummy head node to keep things simple.
 */
extern queue_t *create_queue();

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
extern void push(queue_t **queue, process_t *process);

/**
 * Removes and returns the current process from the specified node in the queue.
 * 
 * This function takes a pointer to a node (within a queue) and removes that node from the queue.
 * It handles the connections of surrounding nodes to maintain the integrity of the queue.
 * Finally, it frees the memory allocated for the removed node and returns the process it contained.
 * 
 * @param queue A double pointer to the node to be removed. This allows the function to modify
 *              the caller's pointer, particularly useful when removing the head of the queue. 
 *              Give us flexibility to remove any node in the queue.
 * @return The process contained within the removed node. If the queue is empty or the pointer is
 *         NULL, it returns NULL.
 */
extern process_t *pop(queue_t **queue);

#endif /* QUEUE_H_ */
