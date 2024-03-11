#ifndef QUEUE_H_
#define QUEUE_H_

#define ALLOCATED_MEMORY 1024

typedef struct {
    process_t *process;
    struct node_t *next;
} node_t;


typedef struct {
    int printers;
    int scanners;
    int modems;
    int cds;
    int memory_available;
    int realtime_memory;
    int memory[ALLOCATED_MEMORY];
} resource_t;

typedef struct {
    int arrival_time;
    int priority;
    int processor_time;
    int mbytes;
    int printers;
    int scanners;
    int modems;
    int cds;
    int mem_index;
} process_t;

extern void push(node_t *queue, process_t *process);

extern process_t *pop(node_t *queue);

#endif /* QUEUE_H_ */