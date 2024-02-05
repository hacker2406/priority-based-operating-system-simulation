#include <stdio.h>
#include <stdbool.h>

int MAX_PROCESSES = 10;
int TIME_QUANTUM_HIGH_PRIORITY = 4;
int TIME_QUANTUM_LOW_PRIORITY = 6;

// Structure to represent a process
struct Process {
    int id;
    int burst_time;
    int io_time;
    int priority; // 0 for high priority, 1 for low priority
    bool completed;
};

// Function to simulate CPU burst
void simulate_cpu_burst(struct Process *process, int time_quantum) {
    bool is_last_time_quantum = process->burst_time <= time_quantum;
    
    if (is_last_time_quantum) {
        printf("Process %d is running (CPU burst time: %d)\n", process->id, process->burst_time);
        process->completed = true;
    } else {
        printf("Process %d is running (CPU burst time: %d)\n", process->id, time_quantum);
        process->burst_time -= time_quantum;
    }

    if (is_last_time_quantum && process->io_time > 0) {
        printf("Process %d is performing I/O (I/O time: %d)\n", process->id, process->io_time);
        int i;
        for (i = 0; i < process->io_time; i++) {
            printf("I/O step %d for Process %d\n", i + 1, process->id);
        }
    }
    
    if (is_last_time_quantum) {
        printf("Process %d has completed.\n", process->id);
    }
}


// Function to age processes in the low priority queue
void age_processes(struct Process *processes, int num_processes) {
    int i;
    for (i = 0; i < num_processes; i++) {
        if (!processes[i].completed && processes[i].priority == 1) {
            processes[i].priority = 0; // Move to high priority
        }
    }
}

int main() {
    printf("______________________________________\n");

    int num_processes;
    do {
        printf("\nEnter the number of processes (up to %d): ", MAX_PROCESSES);
        scanf("%d", &num_processes);
    } while (num_processes <= 0 || num_processes > MAX_PROCESSES);

    // Initialize processes
    struct Process processes[MAX_PROCESSES];
    int i;
    for (i = 0; i < num_processes; i++) {
        printf("Enter burst time for process %d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
        printf("Does process %d need I/O? (0 for no, 1 for yes): ", i + 1);
        scanf("%d", &processes[i].io_time);

        processes[i].id = i + 1;
        processes[i].completed = false;
        processes[i].priority = 0; // Initially, set all processes to high priority
    }

    int step = 1;
    int current_process_index = 0;

    while (1) {
        struct Process *current_process = &processes[current_process_index];

        // Display the current step and process
        printf("Step %d: ", step);
        if (current_process->priority == 0) {
            printf("High Priority - ");
        } else {
            printf("Low Priority - ");
        }

        // Execute the process based on its priority
        if (current_process->priority == 0) {
            if (current_process->burst_time <= TIME_QUANTUM_HIGH_PRIORITY) {
                simulate_cpu_burst(current_process, current_process->burst_time);
            } else {
                simulate_cpu_burst(current_process, TIME_QUANTUM_HIGH_PRIORITY);
            }
        } else {
            if (current_process->burst_time <= TIME_QUANTUM_LOW_PRIORITY) {
                simulate_cpu_burst(current_process, current_process->burst_time);
            } else {
                simulate_cpu_burst(current_process, TIME_QUANTUM_LOW_PRIORITY);
            }
        }

       

        // Update process priority and requeue it based on the remaining burst time
        if (current_process->burst_time > 0) {
            if (current_process->priority == 0) {
                current_process->priority = 1; // Move to low priority
                age_processes(processes, num_processes);
            }
        } else {
            current_process->completed = true;
        }

        // Find the next process to execute
        int next_process_index = (current_process_index + 1) % num_processes;
        int processes_completed = 0;
        while (processes[next_process_index].completed && processes_completed < num_processes) {
            next_process_index = (next_process_index + 1) % num_processes;
            processes_completed++;
        }

        if (processes_completed == num_processes) {
            // All processes completed
            break;
        }

        current_process_index = next_process_index;
        printf("Switching to process %d\n", processes[current_process_index].id);
        step++;
    }

    return 0;
}
