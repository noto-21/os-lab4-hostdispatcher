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
extern void push(queue_t **queue, process_t *process) {
    // Dynamically allocate memory for a new node.
    node_t *new_node = (node_t*) malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // process_t *proc = process;
    
    // printf("(Before) Arrival Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", proc->arrival_time, proc->priority, proc->processor_time, proc->mbytes, proc->printers, proc->scanners, proc->modems, proc->cds);

    // Dynamically allocate memory for a new node.
    node_t *new_node = (node_t*) malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the new_node with process and NULL pointers for prev and next.
    new_node->process = process; // Assign process to the new node.
    new_node->next = NULL; // This new node will be the last node, so next is NULL.
    new_node->prev = NULL; // This will be updated below if it's not the first node.

    if (*queue == NULL) {
        // The queue is empty, so this new node is now the queue.
        *queue = new_node;
    } else {
        // Traverse the queue to find the last node.
        node_t *current = *queue;
        while (current->next != NULL) {
            current = current->next;
        }
        
        // Link the new node into the list.
        new_node->prev = current; // Set new_node's prev pointer to the last node.
        current->next = new_node; // Link the last node to the new node.
    }
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
extern process_t *pop(node_t **queue) {
    if (queue == NULL || *queue == NULL || (*queue)->next == NULL) {
        // If the queue is NULL, the queue is empty, or there is no next node, return NULL.
        return NULL;
    }

    // The node to be removed is the one immediately following the current node.
    node_t *node_to_remove = (*queue)->next;

    // Extract the process from the node to be removed.
    process_t *return_process = node_to_remove->process;

    // Adjust the next pointer of the current node to bypass the node to be removed.
    (*queue)->next = node_to_remove->next;

    // If there is a node after the one being removed, adjust its prev pointer too.
    if (node_to_remove->next != NULL) {
        node_to_remove->next->prev = *queue;
    }

    // Free the memory allocated for the node that's being removed.
    free(node_to_remove);

    // Return the process that was stored in the removed node.
    return return_process;
}
