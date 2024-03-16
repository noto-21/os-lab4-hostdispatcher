#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "resource.h"
#include "memory.h"
#include "util.h"

#define MAX_PROCESSES 10

queue_t *job_queue, *realtime_queue, *first_priority, *second_priority, *third_priority;

int time;
bool terminate = false;

int main() {
    realtime_queue = (queue_t*) malloc(sizeof(queue_t));
    job_queue = (queue_t*) malloc(sizeof(queue_t));

    first_priority = (queue_t*) malloc(sizeof(queue_t));
    second_priority = (queue_t*) malloc(sizeof(queue_t));
    third_priority = (queue_t*) malloc(sizeof(queue_t));

    available_res = (resource_t*) malloc(sizeof(resource_t));
    available_res->cds = MAX_CDS;
    available_res->scanners = MAX_SCANNERS;
    available_res->modems = MAX_MODEMS;

    // In megabytes
    available_res->memory_available = MAX_JOB_MEMORY;
    available_res->realtime_memory = MAX_REALTIME_MEMORY;

    // Flash memory
    for (int i = 0; i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = -1;
    }

    // Attempt to open the file containing process information.
    FILE *file = fopen("dispatchlist.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "File not found\n");
        exit(EXIT_FAILURE);
    }

    process_t data;

    // Read each line from the file and insert the process data into a queue.
    int len = 0;
    
    process_t input_process_list[MAX_PROCESSES];
    while (fscanf(file, "%d, %d, %d, %d, %d, %d, %d, %d\n", &data.arrival_time, &data.priority, &data.processor_time, &data.mbytes, &data.printers, &data.scanners, &data.modems, &data.cds) == 8) {
        data.mem_index = -1;
        input_process_list[len++] = data;
    }

    process_t dispatch_list[len];
    for (int i = 0; i < len; i++) {
        dispatch_list[i] = input_process_list[i];
    }

    // Close the file after reading the contents.
    fclose(file);

    

    int dispatch_list_len = sizeof(dispatch_list) / sizeof(dispatch_list[0]);
    qsort(dispatch_list, dispatch_list_len, sizeof(process_t), arrivalTime);
    for (int i = 0; i < dispatch_list_len; i ++) {
        process_t *proc = &dispatch_list[i];
        printf("Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", time, proc->priority, proc->processor_time, proc->mbytes, proc->printers, proc->scanners, proc->modems, proc->cds);

        if (proc->priority == 0) {
            push(realtime_queue, proc);
        } else {
            push(job_queue, proc);
        }
    }

    process_t *temp_job_process;
    process_t *temp_first_priority;
    process_t *temp_second_priority;
    process_t *temp_third_priority;

    process_t *running_process = NULL;
    while (!terminate) {
        if (realtime_queue != NULL) {
            if (
                time >= realtime_queue->process->arrival_time && 
                realtime_queue->process->processor_time > 0
            ) {
                running_process = realtime_queue->process;
                if (running_process->mem_index == -1)
                    allocate_memory(REALTIME_MODE, running_process);

                printf("Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", time, running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);

                if (running_process->processor_time-- <= 0) {
                    deallocate_memory(REALTIME_MODE, running_process);

                    queue_t *next_realtime = realtime_queue->next;
                    pop(realtime_queue);
                    if (next_realtime != NULL) {
                        realtime_queue = next_realtime;
                    } else {
                        running_process = NULL;
                    }
                }
            }
        }

        if (running_process == NULL) {
            // Job queue priority
            if (
                available_res->cds > 0 || 
                available_res->scanners > 0 || 
                available_res->modems > 0
            ) {
                if (temp_job_process == NULL)
                    temp_job_process = pop(job_queue);

                if (
                    time >= temp_job_process->arrival_time && 
                    available_res->cds >= temp_job_process->cds && 
                    available_res->scanners >= temp_job_process->scanners && 
                    available_res->modems >= temp_job_process->modems
                ) {
                    if (temp_job_process->mem_index == -1) {
                        allocate_memory(JOB_MODE, temp_job_process);
                        available_res->cds -= temp_job_process->cds;
                        available_res->scanners -= temp_job_process->scanners;
                        available_res->modems -= temp_job_process->modems;
                    }

                    switch (temp_job_process->priority) {
                        case 1:
                            push(first_priority, temp_job_process);
                            break;
                        case 2:
                            push(second_priority, temp_job_process);
                            break;
                        case 3:
                            push(third_priority, temp_job_process);
                            break;
                        default:
                            printf("Invalid priority\n");
                            break;
                    }

                    printf("Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", time, running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                    if (temp_job_process->processor_time-- <= 0) {
                        deallocate_memory(JOB_MODE, temp_job_process);
                        temp_job_process = NULL;
                    }
                }
            }
            
            if (first_priority != NULL) {
                if (
                    time >= first_priority->process->arrival_time && 
                    first_priority->process->processor_time > 0
                ) {
                    if (temp_first_priority == NULL) {
                        queue_t *next_first_priority = first_priority->next;
                        temp_first_priority = pop(first_priority);
                        first_priority = next_first_priority;
                    }

                    if (
                        temp_first_priority->mem_index == -1 && 
                        available_res->cds >= temp_first_priority->cds && 
                        available_res->scanners >= temp_first_priority->scanners && 
                        available_res->modems >= temp_first_priority->modems
                    ) {
                        allocate_memory(JOB_MODE, temp_first_priority);
                        available_res->cds -= temp_first_priority->cds;
                        available_res->scanners -= temp_first_priority->scanners;
                        available_res->modems -= temp_first_priority->modems;
                    }
                    running_process = temp_first_priority;

                    printf("Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", time, running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                    push(second_priority, temp_first_priority);
                    temp_first_priority = NULL;

                    if (running_process->processor_time-- <= 0) {
                        deallocate_memory(JOB_MODE, running_process);
                        available_res->cds += running_process->cds;
                        available_res->scanners += running_process->scanners;
                        available_res->modems += running_process->modems;
                    }

                    running_process = NULL;
                
                }
            }
            
            if (second_priority != NULL) {
                if (
                    time >= second_priority->process->arrival_time && 
                    second_priority->process->processor_time > 0 
                ) {

                    if (temp_second_priority == NULL) {
                        queue_t *next_second_priority = second_priority->next;
                        temp_second_priority = pop(second_priority);
                        second_priority = next_second_priority;
                    }

                    if (
                        temp_second_priority->mem_index == -1 && 
                        available_res->cds >= temp_second_priority->cds && 
                        available_res->scanners >= temp_second_priority->scanners && 
                        available_res->modems >= temp_second_priority->modems
                    ) {
                        allocate_memory(JOB_MODE, temp_second_priority);
                        available_res->cds -= temp_second_priority->cds;
                        available_res->scanners -= temp_second_priority->scanners;
                        available_res->modems -= temp_second_priority->modems;
                    }
                    running_process = temp_second_priority;

                    printf("Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", time, running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                    push(third_priority, temp_second_priority);
                    temp_second_priority = NULL;

                    if (running_process->processor_time-- <= 0) {
                        deallocate_memory(JOB_MODE, running_process);
                        available_res->cds += running_process->cds;
                        available_res->scanners += running_process->scanners;
                        available_res->modems += running_process->modems;
                    }

                    running_process = NULL;
                }
            }


            
            if (third_priority != NULL) {
                if (
                    time >= third_priority->process->arrival_time && 
                    third_priority->process->processor_time > 0 
                ) {

                    if (temp_third_priority == NULL) {
                        queue_t *next_third_priority = third_priority->next;
                        temp_third_priority = pop(third_priority);
                        third_priority = next_third_priority;
                    }

                    if (
                        temp_third_priority->mem_index == -1 && 
                        available_res->cds >= temp_third_priority->cds && 
                        available_res->scanners >= temp_third_priority->scanners && 
                        available_res->modems >= temp_third_priority->modems
                    ) {
                        allocate_memory(JOB_MODE, temp_third_priority);
                        available_res->cds -= temp_third_priority->cds;
                        available_res->scanners -= temp_third_priority->scanners;
                        available_res->modems -= temp_third_priority->modems;
                    }
                    running_process = temp_third_priority;

                    printf("Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", time, running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                    push(third_priority, temp_third_priority);
                    temp_third_priority = NULL;

                    if (running_process->processor_time-- <= 0) {
                        deallocate_memory(JOB_MODE, running_process);
                        available_res->cds += running_process->cds;
                        available_res->scanners += running_process->scanners;
                        available_res->modems += running_process->modems;
                    }

                    running_process = NULL;
                }
            }

        }

        if (
            running_process == NULL && realtime_queue == NULL && job_queue == NULL && 
            first_priority == NULL && second_priority == NULL && third_priority == NULL
        ) {
            terminate = true;
        }
    

        time++;
    }
    
    return 0;
}