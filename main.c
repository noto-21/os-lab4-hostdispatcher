#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "resource.h"
#include "memory.h"
#include "util.h"

#define MAX_PROCESSES 1024

queue_t *job_queue, *realtime_queue, *first_priority, *second_priority, *third_priority;
process_t *running_process = NULL;
resource_t *available_res;

int time;
bool terminate = false;
process_t dispatch_list[MAX_PROCESSES] = {NULL};

int main() {
    realtime_queue = (node_t*) malloc(sizeof(node_t));
    job_queue = (node_t*) malloc(sizeof(node_t));

    first_priority = (node_t*) malloc(sizeof(node_t));
    second_priority = (node_t*) malloc(sizeof(node_t));
    third_priority = (node_t*) malloc(sizeof(node_t));

    available_res = (resource_t*) malloc(sizeof(resource_t));
    available_res->cds = 2;
    available_res->scanners = 1;
    available_res->modems = 1;

    // In megabytes
    available_res->memory_available = 960;
    available_res->realtime_memory = 64;

    // Attempt to open the file containing process information.
    FILE *file = fopen("dispatchlist.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "File not found\n");
        exit(EXIT_FAILURE);
    }

    process_t data;

    // Read each line from the file and insert the process data into a queue.
    int i = 0;
    while (fscanf(file, "%d, %d, %d, %d, %d, %d, %d, %d\n", data.arrival_time, data.priority, data.processor_time, data.mbytes, data.printers, data.scanners, data.modems, data.cds) == 4) {
        data.id = i;
        dispatch_list[i++] = data;
    }

    int dispatch_list_len = sizeof(dispatch_list) / sizeof(dispatch_list[0]);
    qsort(dispatch_list, dispatch_list_len, sizeof(process_t), arrivalTime);
    for (int i = 0; i < dispatch_list_len; i ++) {
        process_t *proc = &dispatch_list[i];
        if (proc->priority == 0) {
            push(realtime_queue, proc);
            // available_res->realtime_memory -= proc->mbytes;
        } else {
            push(job_queue, proc);
        }
    }

    while (!terminate) {
        if (realtime_queue != NULL) {
            if (realtime_queue->process->processor_time > 0) {
                running_process = realtime_queue->process;
                
                available_res->realtime_memory -= running_process->mbytes;
                // available_res->
                running_process->processor_time --;

                if (running_process->processor_time <= 0) {
                    pop(realtime_queue);
                }
            }
        }


        time++;
    }

    // Close the file after reading the contents.
    fclose(file);
    
    return 0;
}