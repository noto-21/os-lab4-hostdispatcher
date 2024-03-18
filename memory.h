#include <stdbool.h>
#include "process.h"
#include "resource.h"

#ifndef MEMORY_H_
#define MEMORY_H_

/**
 * Attempts to allocate the required memory for a given process, based on its mode.
 * This can differentiate between realtime and job modes, adjusting memory allocation accordingly.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param proc A pointer to the process requiring memory allocation.
 * @return True if the memory was successfully allocated, false otherwise.
 */
extern bool allocate_memory(enum MEM_MODE mode, process_t *proc);

/**
 * Frees the memory previously allocated to a process, considering its mode.
 * This ensures that memory is correctly managed and returned to the system.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param proc A pointer to the process whose memory is to be deallocated.
 * @return True if the memory was successfully deallocated, false otherwise.
 */
extern bool deallocate_memory(enum MEM_MODE mode, process_t *proc);

#endif /* MEMORY_H_ */