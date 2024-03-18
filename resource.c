#include <stdio.h>
#include "resource.h"
#include "memory.h"

 // Definition of available resources
resource_t* available_res = NULL;

/**
 * Generates a string representing the difference between two quantities of the same resource.
 * Useful for creating easily understandable logs. For example, differences
 * are formatted as "+5" for positive, "-3" for negative differences.
 * 
 * @param a The initial quantity of the resource.
 * @param b The final quantity of the resource.
 * @return A pointer to a string representation of the difference.
 */
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

/**
 * Checks if the necessary resources, including memory, are available for a process.
 * This considers the process's mode and its specific resource requirements.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param process A pointer to the process for which resources are being checked.
 * @return True if all required resources are available, false otherwise.
 */
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

/**
 * Allocates the required resources, including memory, for a process based on its mode.
 * This function ensures that a process has all necessary resources before execution.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param process A pointer to the process for which resources are being allocated.
 * @return True if resources were successfully allocated, false otherwise.
 */
extern bool allocate_resources(enum MEM_MODE mode, process_t *process) {
    // If the resources are available, allocate the memory and the resources
    if (available_resources(mode, process)) {
        int prev_cds = available_res->cds;
        int prev_scanners = available_res->scanners;
        int prev_modems = available_res->modems;
        int prev_printers = available_res->printers;

        // We use memory as a guideline to check if the resources are available
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

/**
 * Deallocates the resources, including memory, previously allocated to a process.
 * This is crucial for resource management, ensuring resources are returned to the system.
 * 
 * @param mode The mode of the process (REALTIME or JOB).
 * @param process A pointer to the process whose resources are to be deallocated.
 * @return True if resources were successfully deallocated, false otherwise.
 */
extern bool deallocate_resources(enum MEM_MODE mode, process_t *process) {
    int prev_cds = available_res->cds;
    int prev_scanners = available_res->scanners;
    int prev_modems = available_res->modems;
    int prev_printers = available_res->printers;

    // If the resources are available, de-allocate the memory and the resources
    // We use memory as a guideline to check if the resources are available
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