#ifndef QUEUE_H_
#define QUEUE_H_

/** Defines the total amount of memory allocated for use. */
#define ALLOCATED_MEMORY 1024

/** 
 * Structure representing a single node in the queue.
 * Each node contains a process and a pointer to the next node in the queue.
 */
typedef struct {
    /** Pointer to the process data associated with this node. */
    process_t *process;
    /** Pointer to the next node in the queue, NULL if it's the last node. */
    struct node_t *next;
} node_t;

// Alias node_t as queue_t for clarity when used to represent a queue
typedef node_t queue_t;

/** 
 * Structure representing the resources available in the system.
 * This includes various devices and memory configurations.
 */
typedef struct {
    int printers;            /**< Number of printers available. */
    int scanners;            /**< Number of scanners available. */
    int modems;              /**< Number of modems available. */
    int cds;                 /**< Number of CDs available. */
    int memory_available;    /**< Total available memory. */
    int realtime_memory;     /**< Memory allocated for real-time processes. */
    int memory[ALLOCATED_MEMORY]; /**< Array representing memory slots, for detailed memory management. */
} resource_t;

/** 
 * Structure representing a process.
 * It includes details about the process such as arrival time, priority, and resource requirements.
 */
typedef struct {
    int arrival_time;    /**< Time when the process arrives in the system. */
    int priority;        /**< Priority of the process, affects queue positioning. */
    int processor_time;  /**< Time required for processing. */
    int mbytes;          /**< Memory bytes required by the process. */
    int printers;        /**< Number of printers required. */
    int scanners;        /**< Number of scanners required. */
    int modems;          /**< Number of modems required. */
    int cds;             /**< Number of CDs required. */
    int mem_index;       /**< Index in the memory array where this process's memory starts. */
} process_t;

/** 
 * Function to add a process to the queue.
 * @param queue Pointer to the queue (head node).
 * @param process Pointer to the process to be added.
 */
extern void push(queue_t *queue, process_t *process);

/** 
 * Function to remove and return the first process from the queue.
 * @param queue Pointer to the queue (head node).
 * @return Pointer to the removed process.
 */
extern process_t *pop(queue_t *queue);

#endif /* QUEUE_H_ */
