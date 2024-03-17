#include "process.h"
#include "resource.h"

#ifndef MEMORY_H_
#define MEMORY_H_

extern void allocate_memory(enum MEM_MODE mode, process_t *proc);
extern void deallocate_memory(enum MEM_MODE mode, process_t *proc);

extern void allocate_resources(enum MEM_MODE mode, process_t *process);
extern void deallocate_resources(enum MEM_MODE mode, process_t *process);

#endif /* MEMORY_H_ */