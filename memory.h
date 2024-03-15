#include <stdio.h>
#include <stdlib.h>

#ifndef MEMORY_H_
#define MEMORY_H_

/** Defines the total amount of memory allocated for use in megabytes. */
#define ALLOCATED_MEMORY 1024

int mem_index = 0;
int memory[ALLOCATED_MEMORY] = {NULL, NULL};

extern void allocate_memory(int mbytes);

#endif /* MEMORY_H_ */