#include "resource.h"
#include "memory.h"
#include "process.h"

extern char* diff_resources(int a, int b) {
    // We have to use malloc here to allocate some memory for the string, 
    // because you can't return arrays from functions in C.
    // So we need to return a pointer
    char* buffer = malloc(100 * sizeof(char));
    if (buffer == NULL) {
        return NULL; // Allocation failed
    }
    
    int diff = a - b;
    sprintf(buffer, "%+d", diff);
    
    return buffer; 
}

extern bool allocate_memory(enum MEM_MODE mode, process_t *proc) {
    int prev_mem_available = available_res->memory_available;
    int prev_realtime_mem = available_res->realtime_memory;

    if (proc->mem_index != -1) {
        printf("Memory already allocated for process\n");
        return false;
    }

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

    resource_t *res = available_res;
    printf("+ [Allocate Memory]  Memory: %s, Realtime Memory: %s\n", 
        diff_resources(res->memory_available, prev_mem_available), 
        diff_resources(res->realtime_memory, prev_realtime_mem)
    );
    return true;
}

extern bool deallocate_memory(enum MEM_MODE mode, process_t *proc) {
    int prev_mem_available = available_res->memory_available;
    int prev_realtime_mem = available_res->realtime_memory;

    if (proc->mem_index == -1) {
        printf("Memory already deallocated for process\n");
        return false;
    }

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
        
    resource_t *res = available_res;
    printf("- [De-allocate Memory] Memory: %s, Realtime Memory: %s\n", 
        diff_resources(res->memory_available, prev_mem_available), 
        diff_resources(res->realtime_memory, prev_realtime_mem)
    );
    return true;
}

extern bool available_resources(enum MEM_MODE mode, process_t *process) {
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

    return available;
}

extern bool allocate_resources(enum MEM_MODE mode, process_t *process) {
    if (available_resources(mode, process)) {
        int prev_cds = available_res->cds;
        int prev_scanners = available_res->scanners;
        int prev_modems = available_res->modems;
        int prev_printers = available_res->printers;

        if (allocate_memory(mode, process)) {
            available_res->cds -= process->cds;
            available_res->scanners -= process->scanners;
            available_res->modems -= process->modems;
            available_res->printers -= process->printers;

            resource_t *res = available_res;
            printf("+ [Allocate Resources] Printers: %s, Scanners: %s, Modems: %s, CDs: %s\n", 
                diff_resources(res->printers, prev_printers), 
                diff_resources(res->scanners, prev_scanners), 
                diff_resources(res->modems, prev_modems), 
                diff_resources(res->cds, prev_cds)
            );
            return true;
        }
    }
    return false;

}

extern bool deallocate_resources(enum MEM_MODE mode, process_t *process) {
    int prev_cds = available_res->cds;
    int prev_scanners = available_res->scanners;
    int prev_modems = available_res->modems;
    int prev_printers = available_res->printers;

    if (deallocate_memory(mode, process)) {
        available_res->cds += process->cds;
        available_res->scanners += process->scanners;
        available_res->modems += process->modems;
        available_res->printers += process->printers;

        resource_t *res = available_res;
        printf("- [De-allocate Resources] Printers: %s, Scanners: %s, Modems: %s, CDs: %s\n", 
            diff_resources(res->printers, prev_printers), 
            diff_resources(res->scanners, prev_scanners), 
            diff_resources(res->modems, prev_modems), 
            diff_resources(res->cds, prev_cds)
        );
        return true;
    }

    return false;
}