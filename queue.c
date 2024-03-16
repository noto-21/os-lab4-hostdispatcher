#include "queue.h"

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
extern void push(node_t *queue, process_t *process) {
    // Dynamically allocate memory for a new node.
    node_t *new_node = (node_t*) malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Traverse the queue to find the last node.
    node_t *current = queue;
    while (current->next != NULL) {
        current = current->next;
    }
    
    // Assign the process to the new node and adjust pointers to insert the new node into the queue.
    new_node->process = process;     // Point the new_node's process to the passed process.
    new_node->next = NULL;     // Set new_node's next pointer to NULL.
    new_node->prev = current;    // Set new_node's prev pointer to the current node of the queue.
    current->next = new_node;    // Insert the new_node right after the dummy head node.
    queue->prev = current;    // Set the dummy head node's prev pointer to the new_node.
}

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
extern process_t *pop(node_t *queue) {
    if (queue == NULL || queue->next == NULL) {
        // If the queue is empty, return NULL.
        return NULL;
    }
    
    node_t *current_node = queue;   
    node_t *next_node = current_node->next;    
    node_t *previous_node = current_node->prev;       

    // Disconnect the last node from the queue.
    if (next_node != NULL) {
        next_node->prev = previous_node;
        queue = next_node;
    }

    if (previous_node != NULL) {
        previous_node->next = next_node;
        queue = previous_node;
    }

    current_node->next = NULL;
    current_node->prev = NULL;

    // Extract the process from the last node and return it.
    process_t *return_process = current_node->process;
    free(current_node);

    return return_process;
}