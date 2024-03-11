#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Define your FIFO queue functions here, these will most likely be the
// push and pop functions that you declared in your header file

// Pushes a node onto a queue. The queue is accessed from the tail
// instead of the head so pushing is just a simple operation of 
// insertion
extern void push(node_t *queue, process_t *process) {
    node_t *new_node = (node_t*) malloc(sizeof(node_t));
    new_node->process = process;
    new_node->next = queue->next;
    queue->next = new_node;
}


// Pops a process from the queue. Since the queue is accessed through
// the tail instead of the head we need to iterate to the front of the 
// queue to pop the first element that was pushed (FIFO)
extern process_t *pop(node_t *queue){
    process_t *return_process;
    node_t *current_node, *previous_node;
    if (queue->next == NULL){
        return NULL;
    }
    else {
        current_node = queue->next;
        previous_node = queue;
        while(current_node->next != NULL){
            previous_node = current_node;
            current_node = current_node->next;
        }
    }
    previous_node->next = NULL;
    return_process =  current_node->process;
    return return_process;
}