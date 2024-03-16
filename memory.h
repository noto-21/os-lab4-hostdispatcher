#include "process.h"
#include "resource.h"

#ifndef MEMORY_H_
#define MEMORY_H_

extern void allocate_memory(enum MEM_MODE mode, process_t *proc);
extern void deallocate_memory(enum MEM_MODE mode, process_t *proc);

#endif /* MEMORY_H_ */