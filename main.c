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
    realtime_queue = create_queue();
    job_queue = create_queue();

    first_priority = create_queue();
    second_priority = create_queue();
    third_priority = create_queue();

    available_res = (resource_t*) malloc(sizeof(resource_t));
    if (!available_res) {
        fprintf(stderr, "Memory allocation failed to create a new resource struct of available resources\n");
        exit(EXIT_FAILURE);
    }
    available_res->cds = MAX_CDS;
    available_res->scanners = MAX_SCANNERS;
    available_res->modems = MAX_MODEMS;
    available_res->printers = MAX_PRINTERS;

    // In megabytes
    available_res->memory_available = MAX_JOB_MEMORY;
    available_res->realtime_memory = MAX_REALTIME_MEMORY;

    // Flash memory
    for (int i = 0; i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = -1;
    }

    process_t input_process_list[MAX_PROCESSES];
    process_t data;
    int len = 0;

    // Attempt to open the file containing process information.
    FILE *file = fopen("dispatchlist.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "File not found\n");
        exit(EXIT_FAILURE);
    }

    // Read each line from the file and insert the process data the input_process_list (basiacally think of this as the dispatch_list).
    while (fscanf(file, "%d, %d, %d, %d, %d, %d, %d, %d\n", &data.arrival_time, &data.priority, &data.processor_time, &data.mbytes, &data.printers, &data.scanners, &data.modems, &data.cds) == 8 && len < MAX_PROCESSES) {
        // The -1 value indicates that the process has not been allocated memory.
        data.mem_index = -1;
        input_process_list[len++] = data;
    }

    // Close the file after reading the contents.
    fclose(file);
    
    // For efficient memory management shrink the dispatch list to the number of processes actually read from the file.
    process_t dispatch_list[len];
    for (int i = 0; i < len; i++) {
        dispatch_list[i] = input_process_list[i];
    } 

    int dispatch_list_len = sizeof(dispatch_list) / sizeof(dispatch_list[0]);
    qsort(dispatch_list, dispatch_list_len, sizeof(process_t), arrivalTime);
    for (int i = 0; i < dispatch_list_len; i ++) {
        process_t *proc = &dispatch_list[i];

        if (proc->priority == 0) {
            push(&realtime_queue, proc);
        } else {
            push(&job_queue, proc);
        }
    }

    queue_t *current_process = realtime_queue;
    while (current_process != NULL) {
        if (current_process->process != NULL) {
            process_t *proc = current_process->process;
            printf("(Verify) Arrival Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", proc->arrival_time, proc->priority, proc->processor_time, proc->mbytes, proc->printers, proc->scanners, proc->modems, proc->cds);
        } else {
            printf("Process is NULL.\n");
        }
        current_process = current_process->next;
    }

    current_process = job_queue;
    while (current_process != NULL) {
        if (current_process->process != NULL) {
            process_t *proc = current_process->process;
            printf("(Verify) Arrival Time: %d, Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", proc->arrival_time, proc->priority, proc->processor_time, proc->mbytes, proc->printers, proc->scanners, proc->modems, proc->cds);
        } else {
            printf("Process is NULL.\n");
        }
        current_process = current_process->next;
    }

    resource_t *res = available_res;
    printf("(Available Resource) Memory Available: %d, Realtime Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n\n", res->memory_available, res->realtime_memory, res->printers, res->scanners, res->modems, res->cds);

    process_t *temp_job_process = NULL;
    process_t *temp_first_priority = NULL;
    process_t *temp_second_priority = NULL;
    process_t *temp_third_priority = NULL;

    process_t *running_process = NULL;
    process_t *temp_realtime_process = NULL;
    while (!terminate && time < 30) {
        printf("Time: %d\n", time);
        printf("(Available Resource) Memory Available: %d, Realtime Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", res->memory_available, res->realtime_memory, res->printers, res->scanners, res->modems, res->cds);

        if (
            realtime_queue != NULL &&
            realtime_queue->process != NULL &&
            time >= realtime_queue->process->arrival_time && 
            realtime_queue->process->processor_time > 0 && 
            available_res->realtime_memory > 0
        ) {            
            if (temp_realtime_process == NULL) {
                queue_t *next_realtime_process = realtime_queue->next;
                temp_realtime_process = pop(&realtime_queue);
                realtime_queue = next_realtime_process;
            }

            if (temp_realtime_process != NULL) {
                if (temp_realtime_process->mem_index == -1) 
                    allocate_memory(REALTIME_MODE, temp_realtime_process);

                running_process = temp_realtime_process;
                printf("- Running => (Realtime) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                printf("(Available Resource) Memory Available: %d, Realtime Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", res->memory_available, res->realtime_memory, res->printers, res->scanners, res->modems, res->cds);

                if (--running_process->processor_time <= 0) {
                    deallocate_memory(REALTIME_MODE, running_process);
                    // free(temp_realtime_process);
                    temp_realtime_process = NULL;
                    // running_process = NULL;
                }
            }
        }

        // Job queue priority
        if (
            available_res->cds > 0 || 
            available_res->scanners > 0 || 
            available_res->modems > 0 ||
            available_res->printers > 0 
        ) {
            // printf("+ (Before) job_queue priority %d => temp_job_process: %p\n", job_queue->process->priority, temp_job_process);
            if (temp_job_process == NULL)
                temp_job_process = pop(&job_queue);
            
            if (
                job_queue != NULL && 
                job_queue->process != NULL && 
                temp_job_process != NULL
            ) {
                // printf("+ (After) job_queue priority %d => temp_job_process: %p->priority = %d\n", job_queue->process->priority, temp_job_process, temp_job_process->priority);
            } else {
                // printf("+ (After) job_queue is empty\n");
            }

            if (
                temp_job_process != NULL &&
                time >= temp_job_process->arrival_time && 
                available_res->cds >= temp_job_process->cds && 
                available_res->scanners >= temp_job_process->scanners && 
                available_res->modems >= temp_job_process->modems &&
                available_res->printers >= temp_job_process->printers
            ) {
                printf("* (User Job Queue - Debug) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", temp_job_process->priority, temp_job_process->processor_time, temp_job_process->mbytes, temp_job_process->printers, temp_job_process->scanners, temp_job_process->modems, temp_job_process->cds);
                switch (temp_job_process->priority) {
                    case 1:
                        push(&first_priority, temp_job_process);
                        break;
                    case 2:
                        push(&second_priority, temp_job_process);
                        break;
                    case 3:
                        push(&third_priority, temp_job_process);
                        break;
                    default:
                        printf("Invalid priority\n");
                        break;
                }

                temp_job_process = NULL;
            }
        }
     
        if (
            running_process == NULL &&
            first_priority != NULL &&
            first_priority->process != NULL &&
            time >= first_priority->process->arrival_time && 
            first_priority->process->processor_time > 0
        ) {
            if (temp_first_priority == NULL) {
                queue_t *next_first_priority = first_priority->next;
                temp_first_priority = pop(&first_priority);
                first_priority = next_first_priority;
            }

            if (temp_first_priority != NULL) {
                allocate_resources(JOB_MODE, temp_first_priority);
                running_process = temp_first_priority;

                printf("- Running => (1st Priority Queue) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);

                push(&second_priority, temp_first_priority);
                temp_first_priority = NULL;
            }
        }
    
        if (
            running_process == NULL &&
            second_priority != NULL &&
            second_priority->process != NULL &&
            time >= second_priority->process->arrival_time && 
            second_priority->process->processor_time > 0 
        ) {
            if (temp_second_priority == NULL) {
                queue_t *next_second_priority = second_priority->next;
                temp_second_priority = pop(&second_priority);
                second_priority = next_second_priority;
            }

            if (temp_second_priority != NULL) {                    
                allocate_resources(JOB_MODE, temp_second_priority);
                running_process = temp_second_priority;

                printf("- Running => (2nd Priority Queue) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);

                push(&third_priority, temp_second_priority);
                temp_second_priority = NULL;
            }
        }


        if (
            running_process == NULL &&
            third_priority != NULL &&
            third_priority->process != NULL &&
            time >= third_priority->process->arrival_time
        ) {
            // printf("3rd Priority: %d\n", third_priority->process->processor_time);
            if (temp_third_priority == NULL) {
                queue_t *next_third_priority = third_priority->next;
                temp_third_priority = pop(&third_priority);
                third_priority = next_third_priority;
            }

            if (temp_third_priority != NULL) {
                allocate_resources(JOB_MODE, temp_third_priority);
                running_process = temp_third_priority;

                printf("- Running => (3rd Priority Queue) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);

                if (temp_third_priority->processor_time > 0) {
                    push(&third_priority, temp_third_priority);
                }
                temp_third_priority = NULL;
            }
        }

        if (running_process != NULL) {
            printf("(Available Resource) Memory Available: %d, Realtime Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", res->memory_available, res->realtime_memory, res->printers, res->scanners, res->modems, res->cds);
            if (running_process->priority > 0 && --running_process->processor_time <= 0) {
                deallocate_resources(JOB_MODE, running_process);
                printf("[De-allocate Resources] (Available Resource) Memory Available: %d, Realtime Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", res->memory_available, res->realtime_memory, res->printers, res->scanners, res->modems, res->cds);
        
            }

            running_process = NULL;
        }

        if (running_process == NULL) {
            printf("Job Queue: %p, Realtime Queue: %p, 1st Priority: %p, 2nd Priority: %p, 3rd Priority: %p\n", job_queue, realtime_queue, first_priority, second_priority, third_priority);
            if (
                ((
                    (job_queue != NULL && job_queue->process != NULL) &&
                    (realtime_queue != NULL && realtime_queue->process != NULL) &&
                    (first_priority != NULL && first_priority->process != NULL) &&
                    (second_priority != NULL && second_priority->process != NULL) &&
                    (third_priority != NULL && third_priority->process != NULL)
                ) || (
                    realtime_queue == NULL && job_queue == NULL && first_priority == NULL && 
                    second_priority == NULL && third_priority == NULL
                ))
            ) {
                terminate = true;
                printf("Terminating...\n");
            }
        }
    

        time++;
        printf("\n");
    }
    
    return 0;
}