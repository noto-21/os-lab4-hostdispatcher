#include <stdbool.h>
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

extern void allocate_resources(enum MEM_MODE mode, process_t *process) {
    bool available = (
        process != NULL &&
        process->mem_index == -1 && 
        available_res->cds >= process->cds && 
        available_res->scanners >= process->scanners && 
        available_res->modems >= process->modems &&
        available_res->printers >= process->printers
    );

    switch (mode) {
        case JOB_MODE:
            available = available && (available_res->memory_available >= process->mbytes);
            break;
        
        case REALTIME_MODE:
            available = available && (available_res->realtime_memory >= process->mbytes);
            break;
    }

    if (available) {
        allocate_memory(mode, process);
        available_res->cds -= process->cds;
        available_res->scanners -= process->scanners;
        available_res->modems -= process->modems;
        available_res->printers -= process->printers;
    }
}

extern void deallocate_resources(enum MEM_MODE mode, process_t *process) {
    deallocate_memory(mode, process);
    available_res->cds += process->cds;
    available_res->scanners += process->scanners;
    available_res->modems += process->modems;
    available_res->printers += process->printers;
}