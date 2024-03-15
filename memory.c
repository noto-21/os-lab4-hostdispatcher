#include "memory.h"

extern int allocate_memory(int processor_id, int mbytes)  {
    int temp_mem_index = mem_index;
    for (int i = mem_index; i < mem_index + mbytes && i < ALLOCATED_MEMORY; i++) {
        memory[i] = processor_id;
    }

    mem_index += mbytes;
    return temp_mem_index;
}