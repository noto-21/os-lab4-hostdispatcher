#include "resource.h"
#include "memory.h"

extern int allocate_memory(enum MEM_MODE mode, int mbytes)  {
    int temp_mem_index = available_res->mem_index;
    for (int i = temp_mem_index; i < available_res->mem_index + mbytes && i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = 1;
    }

    switch (mode) {
        case MEM_MODE_REALTIME:

    }


    available_res->mem_index += mbytes;
    return temp_mem_index;
}

extern void deallocate_memory(int processor_id, int index_mem, int mbytes)  {
    for (int i = index_mem; i < index_mem + mbytes && i < ALLOCATED_MEMORY; i++) {
        memory[i] = NULL;
    }

    mem_index -= mbytes;
}