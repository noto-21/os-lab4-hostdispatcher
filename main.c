#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "resource.h"
#include "memory.h"
#include "util.h"

#define MAX_PROCESSES 10

queue_t *job_queue, *realtime_queue, *first_priority, *second_priority, *third_priority;

// Time in seconds
int time;
bool terminate = false;

int main() {
    // Initialize the queues
    realtime_queue = create_queue();
    job_queue = create_queue();

    first_priority = create_queue();
    second_priority = create_queue();
    third_priority = create_queue();

    // Initialize the available resources
    available_res = (resource_t*) malloc(sizeof(resource_t));
    if (!available_res) {
        fprintf(stderr, "Memory allocation failed to create a new resource struct of available resources\n");
        exit(EXIT_FAILURE);
    }

    // Set the available resources to their available maximums
    available_res->cds = MAX_CDS;
    available_res->scanners = MAX_SCANNERS;
    available_res->modems = MAX_MODEMS;
    available_res->printers = MAX_PRINTERS;

    // In megabytes
    available_res->memory_available = MAX_JOB_MEMORY;
    available_res->realtime_memory = MAX_REALTIME_MEMORY;

    // Flash/Clear memory
    for (int i = 0; i < ALLOCATED_MEMORY; i++) {
        available_res->memory[i] = -1;
    }

    // `input_process_list` is a temporary list that holds the processes read from the file.
    process_t input_process_list[MAX_PROCESSES];
    process_t data; // Temporary variable to hold the data read from the file.

    // `len` is the length of the data stored in `input_process_list` array from the file.
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
    
    // For efficient memory management, shrink the dispatch list to the number of processes actually read from the file.
    process_t dispatch_list[len];
    for (int i = 0; i < len; i++) {
        dispatch_list[i] = input_process_list[i];
    } 

    // Sort the dispatch list by arrival time.
    int dispatch_list_len = sizeof(dispatch_list) / sizeof(dispatch_list[0]);
    qsort(dispatch_list, dispatch_list_len, sizeof(process_t), arrivalTime);

    // Push the processes into the appropriate queues based off of their priority.
    for (int i = 0; i < dispatch_list_len; i ++) {
        process_t *proc = &dispatch_list[i];

        if (proc->priority == 0) {
            push(&realtime_queue, proc);
        } else {
            push(&job_queue, proc);
        }
    }

    // Verify the contents of the queues and the available resources.
    // This is a debugging step, it's to ensure that the processes were read correctly and that the queues were populated correctly.
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
    printf("(Available Resources) Memory Available: %d, Realtime Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n\n", res->memory_available, res->realtime_memory, res->printers, res->scanners, res->modems, res->cds);

    // Each variable hols the current process that is running per queue. We pop out processes from the queue and store them in these variables.
    // This is to ensure that we don't lose track of the processes that are running, ustil they finish execution.
    // Think of them as the current process per-queue that we're trying to execute.
    // By default the order goes realtime queue (prempt), user jobs queue, first priority queue, second priority queue, third priority queue (round robin).
    
    // The realtime queue is preemptive, so we don't need to worry about the time quantum. It keeps running til completion.
    // The time quantum is 1 second (every iteration of the `time` variable is 1 sec).
    // For user jobs, we use a round-robin approach, so we run the first process in the first priority queue, then the first process in the second priority queue, 
    // then the first process in the third priority queue, then the second process in the first priority queue, and so on.
    // If the 3rd priority queue still has processes, we push said process to the end of the queue, and do this for every iteration of the `time` variable, until the process finishes execution.
    // Note: we only deallocate resources (memory included) once the process finishes execution.

    // One all the queue are empty and all the processes have finished execution, we terminate the program.
    
    // Running process stores the running process in general (realtime or job queue), we use it to stop job processes from running.
    // This helps enforce the time quantum for job processes.

    // Remember: the realtime queue is preemptive, so we don't need to worry about the time quantum. It keeps running til completion.
    process_t *running_process = NULL;

    process_t *temp_job_process = NULL;
    process_t *temp_realtime_process = NULL;

    process_t *temp_first_priority = NULL;
    process_t *temp_second_priority = NULL;
    process_t *temp_third_priority = NULL;

    // We limit the time to 100 seconds to avoid an infinite loop.
    while (!terminate && time < 100) {
        printf("Time: %d - (Available Resources) Memory Available: %d, Realtime Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", time, res->memory_available, res->realtime_memory, res->printers, res->scanners, res->modems, res->cds);

        // Realtime queue priority
        // If the queue is empty or the process has not arrived yet, we don't need to do anything.
        if (
            realtime_queue != NULL &&
            realtime_queue->process != NULL
        ) {
            // Pop the first process from the realtime queue, and run it. 
            // But only if the the process menat to be running for this queue has completed execution.
            if (temp_realtime_process == NULL) {
                queue_t *next_realtime_process = realtime_queue->next;
                temp_realtime_process = pop(&realtime_queue);
                realtime_queue = next_realtime_process;
            }

            // Only run the process if it has arrived and it still has processor time left.
            if (
                temp_realtime_process != NULL &&
                time >= temp_realtime_process->arrival_time
            ) {
                if (temp_realtime_process->processor_time > 0) {
                    // If the process has not been allocated memory, allocate it.
                    if (temp_realtime_process->mem_index == -1) {
                        allocate_memory(REALTIME_MODE, temp_realtime_process);
                    }

                    running_process = temp_realtime_process;
                    printf("= Running => (Realtime) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                }
                
                // If the process has finished execution, deallocate the memory (resources are not required for the realtime queue).
                if (--temp_realtime_process->processor_time <= 0) {
                    deallocate_memory(REALTIME_MODE, temp_realtime_process);
                    temp_realtime_process = NULL;
                }
            }
        }

        // Pop the first process from the job queue, if there is one, if not `temp_job_process` will be `NULL`, and we don't need to do anything.
        if (temp_job_process == NULL)
            temp_job_process = pop(&job_queue);

        // Job queue priority, at a bare minimum we need to have the resources available, and the process needs to have arrived.
        if (
            temp_job_process != NULL &&
            time >= temp_job_process->arrival_time && 
            available_resources(JOB_MODE, temp_job_process)
        ) {
            printf("~ Job Queue (%p), Head process: %p->priority = %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", job_queue, temp_job_process, temp_job_process->priority, temp_job_process->processor_time, temp_job_process->mbytes, temp_job_process->printers, temp_job_process->scanners, temp_job_process->modems, temp_job_process->cds);
            switch (temp_job_process->priority) {
                case 1:
                    // Push the process to the first priority queue, and set the `temp_job_process` to `NULL`.
                    push(&first_priority, temp_job_process);
                    printf("* [Move Job Queue Process to 1st Priority Queue]\n");
                    break;
                case 2:
                    // Push the process to the second priority queue, and set the `temp_job_process` to `NULL`.
                    push(&second_priority, temp_job_process);
                    printf("* [Move Job Queue Process to 2nd Priority Queue]\n");
                    break;
                case 3:
                    // Push the process to the third priority queue, and set the `temp_job_process` to `NULL`.
                    push(&third_priority, temp_job_process);
                    printf("* [Move Job Queue Process to 3rd Priority Queue]\n");
                    break;
                default:
                    printf("Invalid priority\n");
                    break;
            }

            temp_job_process = NULL;
        }
    

        // First priority queue priority
        if (
            // Don't run if another process is running or the queue is empty
            // This is to ensure that we don't run multiple processes at the same time.
            // It's also a way to enforce the time quantum for job processes.
            running_process == NULL &&
            first_priority != NULL &&
            first_priority->process != NULL
         ) {
            // Pop the first process from the first priority queue, and run it. 
            // Reset the current running process for this queue after 1 second, and run the next process in the queue. 
            // Push the process to the 2nd priority queue.
            if (temp_first_priority == NULL) {
                queue_t *next_first_priority = first_priority->next;
                temp_first_priority = pop(&first_priority);
                first_priority = next_first_priority;
            }

            // Only run the process if it has arrived and it still has processor time left.
            if (
                temp_first_priority != NULL &&
                time >= temp_first_priority->arrival_time && 
                temp_first_priority->processor_time > 0
            ) {
                allocate_resources(JOB_MODE, temp_first_priority);
                // Set the running process to the current process from the first priority queue.
                running_process = temp_first_priority;

                printf("= Running => (1st Priority Queue) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                push(&second_priority, temp_first_priority);
                printf("* [Move to 2nd Priority Queue]\n");

                temp_first_priority = NULL;
            }
        }

        // Second priority queue priority
        if (
            // Don't run if another process is running or the queue is empty
            // This is to ensure that we don't run multiple processes at the same time.
            // It's also a way to enforce the time quantum for job processes.
            running_process == NULL &&
            second_priority != NULL &&
            second_priority->process != NULL
        ) {
            // Pop the first process from the second priority queue, and run it. 
            // Reset the current running process for this queue after 1 second, and run the next process in the queue. 
            // Push the process to the 3rd priority queue.
            if (temp_second_priority == NULL) {
                queue_t *next_second_priority = second_priority->next;
                temp_second_priority = pop(&second_priority);
                second_priority = next_second_priority;
            }

            // Only run the process if it has arrived and it still has processor time left.
            if (
                temp_second_priority != NULL &&
                time >= temp_second_priority->arrival_time && 
                temp_second_priority->processor_time > 0
            ) {
                allocate_resources(JOB_MODE, temp_second_priority);
                // Set the running process to the current process from the second priority queue.
                running_process = temp_second_priority;

                printf("= Running => (2nd Priority Queue) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                push(&third_priority, temp_second_priority);
                printf("* [Move to 3rd Priority Queue]\n");

                temp_second_priority = NULL;
            }
        }

        // Third priority queue priority
        // The 2rd priority queue is a round-robin queue, so we need to run the first process in the queue, then the second, then the third, and so on.
        // If the 3rd priority queue still has processes, we push said process to the end of the queue, and do this for every iteration of the `time` variable, until the process finishes execution.
        if (
            // Don't run if another process is running or the queue is empty
            // This is to ensure that we don't run multiple processes at the same time.
            // It's also a way to enforce the time quantum for job processes.
            running_process == NULL &&
            third_priority != NULL &&
            third_priority->process != NULL
        ) {
            // Pop the first process from the third priority queue, and run it. Reset the current running process for this queue after 1 second, and run the next process in the queue. Push the process to the end of the queue.
            if (temp_third_priority == NULL) {
                queue_t *next_third_priority = third_priority->next;
                temp_third_priority = pop(&third_priority);
                third_priority = next_third_priority;
            }

            // Only run the process if it has arrived.
            if (
                temp_third_priority != NULL &&
                time >= temp_third_priority->arrival_time
            ) {
                // IF the process still has processor time left, run it, then push it to the end of the queue.
                if (temp_third_priority->processor_time > 0) {
                    allocate_resources(JOB_MODE, temp_third_priority);
                    running_process = temp_third_priority;

                    printf("= Running => (3rd Priority Queue) Priority: %d, Processor Time: %d, Memory: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n", running_process->priority, running_process->processor_time, running_process->mbytes, running_process->printers, running_process->scanners, running_process->modems, running_process->cds);
                    push(&third_priority, temp_third_priority);
                    printf("* [Round-robin, push to end of 3rd Priority Queue]\n");
                }

                temp_third_priority = NULL;
            }
        }

        // If the running process has finished execution, deallocate the memory and resources.
        if (running_process != NULL) {
            if (
                running_process->priority > 0 && 
                --running_process->processor_time <= 0
            ) {
                deallocate_resources(JOB_MODE, running_process);
            }

            running_process = NULL;
        }

        // If all the queues are empty and all the processes have finished execution, we terminate the program.
        if (running_process == NULL) {
            printf("Queues => Realtime Queue: %p, Job Queue: %p, 1st Priority: %p, 2nd Priority: %p, 3rd Priority: %p\n", realtime_queue, job_queue, first_priority, second_priority, third_priority);
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