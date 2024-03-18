#include "resource.h"
#include "memory.h"
#include "process.h"

/**
 * Attempts to allocate the required memory for a given process, based on its mode.
 * This can differentiate between realtime and job modes, adjusting memory allocation accordingly.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param proc A pointer to the process requiring memory allocation.
 * @return True if the memory was successfully allocated, false otherwise.
 */
extern bool allocate_memory(enum MEM_MODE mode, process_t *proc) {
    int prev_mem_available = available_res->memory_available;
    int prev_realtime_mem = available_res->realtime_memory;

    // We use -1 to indicate that the memory is not allocated
    // If the memory index is not -1, then the memory is already allocated
    // We can't allocate more memory than what is already allocated
    if (proc->mem_index != -1) {
        printf("Memory already allocated for process\n");
        return false;
    }

    int temp_mem_index = available_res->mem_index;
    for (int i = temp_mem_index; i < temp_mem_index + proc->mbytes && i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = 1;
    }

    // Using the mode substract the memory from the correct available resource in this case either realtime or job
    switch (mode) {
        case REALTIME_MODE:
            if (available_res->realtime_memory > 0) {
                available_res->realtime_memory -= proc->mbytes;
            }
            break;

        case JOB_MODE:
            if (available_res->memory_available > 0) {
                available_res->memory_available -= proc->mbytes;
            }
            break;
    }

    // Update the process memory index and the available memory index
    proc->mem_index = temp_mem_index;
    if (available_res->mem_index < ALLOCATED_MEMORY - proc->mbytes)
        available_res->mem_index += proc->mbytes;

    resource_t *res = available_res;
    printf("+ [Allocate Memory]  Memory: %s, Realtime Memory: %s\n", 
        diff_resources(res->memory_available, prev_mem_available), 
        diff_resources(res->realtime_memory, prev_realtime_mem)
    );
    return true;
}

/**
 * Frees the memory previously allocated to a process, considering its mode.
 * This ensures that memory is correctly managed and returned to the system.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param proc A pointer to the process whose memory is to be deallocated.
 * @return True if the memory was successfully deallocated, false otherwise.
 */
extern bool deallocate_memory(enum MEM_MODE mode, process_t *proc) {
    int prev_mem_available = available_res->memory_available;
    int prev_realtime_mem = available_res->realtime_memory;

    // We use -1 to indicate that the memory is not allocated
    // If the memory index is -1, then the memory is already de-allocated
    // Thus there is no need to de-allocate it again
    if (proc->mem_index == -1) {
        printf("Memory already deallocated for process\n");
        return false;
    }

    int temp_mem_index = proc->mem_index;
    for (int i = temp_mem_index; i < proc->mem_index + proc->mbytes && i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = -1;
    }

    // Using the mode add the memory back to the correct available resource in this case either realtime or job
    switch (mode) {
        case REALTIME_MODE:
            if (available_res->realtime_memory < MAX_REALTIME_MEMORY) {
                available_res->realtime_memory += proc->mbytes;
            }
            break;

        case JOB_MODE:
            if (available_res->memory_available < MAX_JOB_MEMORY) {
                available_res->memory_available += proc->mbytes;
            }
            break;
    }

    // Update the process memory index and the available memory index
    proc->mem_index = -1;
    if (available_res->mem_index > 0)
        available_res->mem_index -= proc->mbytes;
        
    resource_t *res = available_res;
    printf("- [De-allocate Memory] Memory: %s, Realtime Memory: %s\n", 
        diff_resources(res->memory_available, prev_mem_available), 
        diff_resources(res->realtime_memory, prev_realtime_mem)
    );
    return true;
}
