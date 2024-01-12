#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sched.h> // Include for scheduling functions
#include <stdint.h> // Include for uint32_t

// Function to perform the counting task
void performCounting() {
    uint32_t count = 0;
    while (count < (1U << 32)) {
        count++;
    }
}

int main() {
    int pid1, pid2, pid3;
    struct timespec start_time1, end_time1, start_time2, end_time2, start_time3, end_time3;
    double execution_time1, execution_time2, execution_time3;

    // Create Process 1
    pid1 = fork();
    if (pid1 == 0) {
        struct sched_param param;
        param.sched_priority = 0;
        sched_setscheduler(0, SCHED_OTHER, &param);

        // Call the counting function
        performCounting();

        exit(0);
    }

    // Create Process 2
    pid2 = fork();
    if (pid2 == 0) {
        struct sched_param param;
        param.sched_priority = 0;
        sched_setscheduler(0, SCHED_RR, &param);

        // Call the counting function
        performCounting();

        exit(0);
    }

    // Create Process 3
    pid3 = fork();
    if (pid3 == 0) {
        struct sched_param param;
        param.sched_priority = 0;
        sched_setscheduler(0, SCHED_FIFO, &param);

        // Call the counting function
        performCounting();

        exit(0);
    }

    // This is the parent process.

    clock_gettime(CLOCK_MONOTONIC, &start_time1);
    waitpid(pid1, NULL, 0);
    clock_gettime(CLOCK_MONOTONIC, &end_time1);

    clock_gettime(CLOCK_MONOTONIC, &start_time2);
    waitpid(pid2, NULL, 0);
    clock_gettime(CLOCK_MONOTONIC, &end_time2);

    clock_gettime(CLOCK_MONOTONIC, &start_time3);
    waitpid(pid3, NULL, 0);
    clock_gettime(CLOCK_MONOTONIC, &end_time3);

    execution_time1 = (end_time1.tv_sec - start_time1.tv_sec) +
                     (end_time1.tv_nsec - start_time1.tv_nsec) / 1e9;

    execution_time2 = (end_time2.tv_sec - start_time2.tv_sec) +
                     (end_time2.tv_nsec - start_time2.tv_nsec) / 1e9;

    execution_time3 = (end_time3.tv_sec - start_time3.tv_sec) +
                     (end_time3.tv_nsec - start_time3.tv_nsec) / 1e9;

    printf("Process 1 execution time: %f seconds\n", execution_time1);
    printf("Process 2 execution time: %f seconds\n", execution_time2);
    printf("Process 3 execution time: %f seconds\n", execution_time3);

    // Generate histograms using Python (example):
    system("python generate_histograms.py");

    return 0;
}
