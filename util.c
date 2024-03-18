#include "util.h"
#include "process.h"

extern int arrivalTime(const void* a, const void* b) {
    process_t *process_a = (process_t *)a;
    process_t *process_b = (process_t *)b;
    return process_a->arrival_time - process_b->arrival_time; // Ascending order
}