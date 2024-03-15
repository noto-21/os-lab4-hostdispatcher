#include "memory.h"

#ifndef RESOURCE_H_
#define RESOURCE_H_

typedef struct {
    int printers;            /** Number of printers available. */
    int scanners;            /** Number of scanners available. */
    int modems;              /** Number of modems available. */
    int cds;                 /** Number of CDs available. */
    int memory_available;    /** Total available memory. */
    int realtime_memory;     /** Memory allocated for real-time processes. */
    int memory[ALLOCATED_MEMORY]; /** Array representing memory slots, for detailed memory management. */
} resource_t;

#endif /* RESOURCE_H_ */
