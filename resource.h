#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "memory.h"

#define ALLOCATED_MEMORY 1024
#define MAX_CDS 2
#define MAX_SCANNERS 1
#define MAX_MODEMS 1

typedef struct {
    int printers;            /** Number of printers available. */
    int scanners;            /** Number of scanners available. */
    int modems;              /** Number of modems available. */
    int cds;                 /** Number of CDs available. */
    int memory_available;    /** Total available memory. */
    int realtime_memory;     /** Memory allocated for real-time processes. */
    int memory[ALLOCATED_MEMORY]; /** Array representing memory slots, for detailed memory management. */
    int mem_index;
} resource_t;

resource_t *available_res;

#endif /* RESOURCE_H_ */
