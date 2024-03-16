#include <stdio.h>
#include <stdlib.h>

#ifndef MEMORY_H_
#define MEMORY_H_

#include "resource.h"
#include "process.h"

/** Defines the total amount of memory allocated for use in megabytes. */
#define MAX_REALTIME_MEMORY 64
#define MAX_JOB_MEMORY 960

#define REALTIME_MODE 0
#define JOB_MODE 1

enum MEM_MODE {
    REALTIME = REALTIME_MODE,
    JOB = JOB_MODE
};

extern void allocate_memory(enum MEM_MODE mode, process_t *proc);
extern void deallocate_memory(enum MEM_MODE mode, process_t *proc);

#endif /* MEMORY_H_ */