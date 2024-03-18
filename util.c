#include "util.h"
#include "process.h"

// A predicate function meant to sort the dispatch_list using the arrival times of processes
extern int arrivalTime(const void* a, const void* b) {
    process_t *process_a = (process_t *)a;
    process_t *process_b = (process_t *)b;
    return process_a->arrival_time - process_b->arrival_time; // Ascending order
}