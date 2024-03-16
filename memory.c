#include "resource.h"
#include "memory.h"
#include "process.h"

extern void allocate_memory(enum MEM_MODE mode, process_t *proc) {
    int temp_mem_index = available_res->mem_index;
    for (int i = temp_mem_index; i < temp_mem_index + proc->mbytes && i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = 1;
    }

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

    proc->mem_index = temp_mem_index;
    if (available_res->mem_index < ALLOCATED_MEMORY - proc->mbytes)
        available_res->mem_index += proc->mbytes;
}

extern void deallocate_memory(enum MEM_MODE mode, process_t *proc) {
    int temp_mem_index = proc->mem_index;
    for (int i = temp_mem_index; i < proc->mem_index + proc->mbytes && i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = -1;
    }

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

    proc->mem_index = -1;
    if (available_res->mem_index > 0)
        available_res->mem_index -= proc->mbytes;
}