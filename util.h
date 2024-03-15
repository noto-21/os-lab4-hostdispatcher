#include "process.h"

#ifndef UTIL_H_
#define UTIL_H_

extern int arrivalTime(const void* a, const void* b) {
    process_t *process_a = (process_t *)a;
    process_t *process_b = (process_t *)b;
    return process_a->arrival_time - process_b->arrival_time; // Ascending order
}

#endif /* UTIL_H_ */