#ifndef RESOURCE_H_
#define RESOURCE_H_

/** Defines the total amount of memory allocated for use in megabytes. */
#define ALLOCATED_MEMORY 1024

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

#endif /* RESOURCE_H_ */
