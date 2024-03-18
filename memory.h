#include <stdbool.h>
#include "process.h"
#include "resource.h"

#ifndef MEMORY_H_
#define MEMORY_H_

extern char* diff_resources(int a, int b);

extern bool allocate_memory(enum MEM_MODE mode, process_t *proc);
extern bool deallocate_memory(enum MEM_MODE mode, process_t *proc);

extern bool available_resources(enum MEM_MODE mode, process_t *process);
extern bool allocate_resources(enum MEM_MODE mode, process_t *process);
extern bool deallocate_resources(enum MEM_MODE mode, process_t *process);

#endif /* MEMORY_H_ */