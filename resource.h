#ifndef RESOURCE_H_
#define RESOURCE_H_

/** Defines the total amount of memory allocated for use in megabytes. */
#define MAX_REALTIME_MEMORY 64
#define MAX_JOB_MEMORY 960
#define ALLOCATED_MEMORY 1024

#define MAX_CDS 2
#define MAX_PRINTERS 2

#define MAX_SCANNERS 1
#define MAX_MODEMS 1

#define REALTIME_MODE 0
#define JOB_MODE 1

// Useful for identifying what process mode we are in
extern enum MEM_MODE {
    REALTIME = REALTIME_MODE,
    JOB = JOB_MODE
} MEM_MODE;

// Resource struct, stores the available resources
// We really only use one instance of this struct
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

// Available resources
extern resource_t* available_res;

#endif /* RESOURCE_H_ */
