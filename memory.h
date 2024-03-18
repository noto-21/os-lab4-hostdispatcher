#include <stdbool.h>
#include "process.h"
#include "resource.h"

#ifndef MEMORY_H_
#define MEMORY_H_

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
 * Attempts to allocate the required memory for a given process, based on its mode.
 * This can differentiate between realtime and job modes, adjusting memory allocation accordingly.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param proc A pointer to the process requiring memory allocation.
 * @return True if the memory was successfully allocated, false otherwise.
 */
extern bool allocate_memory(enum MEM_MODE mode, process_t *proc);

// De-allocate memory for a process given the mode (realtime or job) and the process
/**
 * Frees the memory previously allocated to a process, considering its mode.
 * This ensures that memory is correctly managed and returned to the system.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param proc A pointer to the process whose memory is to be deallocated.
 * @return True if the memory was successfully deallocated, false otherwise.
 */
extern bool deallocate_memory(enum MEM_MODE mode, process_t *proc);

// Check if the resources (including memory) are available for a process taking into account the mode (realtime or job)
extern bool available_resources(enum MEM_MODE mode, process_t *process);

// Allocate resources (memory included) for a process taking into account the mode (realtime or job)
extern bool allocate_resources(enum MEM_MODE mode, process_t *process);

// De-allocate resources (memory included) for a process taking into account the mode (realtime or job)
extern bool deallocate_resources(enum MEM_MODE mode, process_t *process);

#endif /* MEMORY_H_ */