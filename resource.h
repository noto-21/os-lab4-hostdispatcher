#include <stdbool.h>
#include "process.h"

#ifndef RESOURCE_H_
#define RESOURCE_H_

/** Defines the total amount of memory allocated for use in megabytes. */
#define MAX_REALTIME_MEMORY 64
#define MAX_JOB_MEMORY 960
#define ALLOCATED_MEMORY 1024

#define MAX_CDS 2
#define MAX_PRINTERS 2

#define MAX_SCANNERS 1
#define MAX_MODEMS 1

#define REALTIME_MODE 0
#define JOB_MODE 1

// Useful for identifying what process mode we are in
extern enum MEM_MODE {
    REALTIME = REALTIME_MODE,
    JOB = JOB_MODE
} MEM_MODE;

// Resource struct, stores the available resources
// We really only use one instance of this struct
typedef struct {
    int printers;            /** Number of printers available. */
    int scanners;            /** Number of scanners available. */
    int modems;              /** Number of modems available. */
    int cds;                 /** Number of CDs available. */
    int memory_available;    /** Total available memory. */
    int realtime_memory;     /** Memory allocated for real-time processes. */
    int memory[ALLOCATED_MEMORY]; /** Array representing memory slots, for detailed memory management. */
    int mem_index;
} resource_t;

// Available resources
extern resource_t* available_res;

/**
 * Generates a string representing the difference between two quantities of the same resource.
 * Useful for creating easily understandable logs. For example, differences
 * are formatted as "+5" for positive, "-3" for negative differences.
 * 
 * @param a The initial quantity of the resource.
 * @param b The final quantity of the resource.
 * @return A pointer to a string representation of the difference.
 */
extern char* diff_resources(int a, int b);

/**
 * Checks if the necessary resources, including memory, are available for a process.
 * This considers the process's mode and its specific resource requirements.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param process A pointer to the process for which resources are being checked.
 * @return True if all required resources are available, false otherwise.
 */
extern bool available_resources(enum MEM_MODE mode, process_t *process);

/**
 * Allocates the required resources, including memory, for a process based on its mode.
 * This function ensures that a process has all necessary resources before execution.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param process A pointer to the process for which resources are being allocated.
 * @return True if resources were successfully allocated, false otherwise.
 */
extern bool allocate_resources(enum MEM_MODE mode, process_t *process);

/**
 * Deallocates the resources, including memory, previously allocated to a process.
 * This is crucial for resource management, ensuring resources are returned to the system.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param process A pointer to the process whose resources are to be deallocated.
 * @return True if resources were successfully deallocated, false otherwise.
 */
extern bool deallocate_resources(enum MEM_MODE mode, process_t *process);

#endif /* RESOURCE_H_ */
