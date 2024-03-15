#include <stdio.h>
#include <stdlib.h>

#ifndef PROCESS_H_
#define PROCESS_H_

typedef struct {
    int arrival_time;    /** Time when the process arrives in the system. */
    int priority;        /** Priority of the process, affects queue positioning. */
    int processor_time;  /** Time required for processing. */
    int mbytes;          /** Memory bytes required by the process. */
    int printers;        /** Number of printers required. */
    int scanners;        /** Number of scanners required. */
    int modems;          /** Number of modems required. */
    int cds;             /** Number of CDs required. */
    int mem_index;       /** Index in the memory array where this process's memory starts. */
} process_t;

#endif /* PROCESS_H_ */
