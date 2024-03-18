#include "queue.h"

/**
 * Creates a new queue.
 * 
 * @param queue Pointer to the head of the queue.
 * 
 * Dynamically allocates memory for a new node and assigns it to the queue pointer.
 * Assumes that the passed 'queue' pointer points to a dummy head node to keep things simple.
 */
extern queue_t *create_queue() {
    // Dynamically allocate memory for a new node of the query.
    node_t *new_node = (node_t*) malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed to create a new queue\n");
        exit(EXIT_FAILURE);
    }
    
    new_node->process = NULL; 
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

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
        fprintf(stderr, "Memory allocation failed to add a new node to the queue\n");
        exit(EXIT_FAILURE);
    }
    
    new_node->process = process;
    new_node->next = NULL;
    new_node->prev = NULL; 

    if (*queue == NULL || (*queue)->process == NULL) {
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
extern process_t *pop(queue_t **queue) {
    // Check if the queue or the target node is NULL. If so, there's nothing to remove.
    if (queue == NULL || *queue == NULL) {
        return NULL;
    }

    // Check if the node contains a process. If not, there's nothing to remove.
    // We don't want to remove the dummy head node, as if we do we would need to re-create the queue from scratch.
    if ((*queue)->process == NULL) {
        return NULL; 
    }

    node_t *node_to_remove = *queue;
    process_t *return_process = node_to_remove->process;

    // If there's a node after the one we're removing, we need to update its 'prev' pointer
    // to skip the removed node, pointing to the previous node of the one being removed.
    if (node_to_remove->next != NULL) {
        node_to_remove->next->prev = node_to_remove->prev;
    }

    // If there's a node before the one we're removing, we update its 'next' pointer to
    // skip the removed node, directly connecting to the next node of the one being removed.
    // If there's no previous node (meaning we're removing the head of the queue), we update
    // the head pointer to point to the next node.
    if (node_to_remove->prev != NULL) {
        node_to_remove->prev->next = node_to_remove->next;
    } else {
        // If removing the head, the next node becomes the new head of the queue.
        *queue = node_to_remove->next;
    }

    // Now that we've detached the node from the queue, we can safely free its memory.
    // What we're freeing here is the `node_t` struct that makes up the node in the queue and not the process itself.
    // As if we remove the process itself, we would lose the reference to it.
    free(node_to_remove);
    return return_process;
}
