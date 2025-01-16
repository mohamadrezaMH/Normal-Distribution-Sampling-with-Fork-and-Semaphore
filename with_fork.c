#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define SAMPLE_SIZE_1 5000
#define SAMPLE_SIZE_2 50000
#define SAMPLE_SIZE_3 500000
#define HIST_SIZE 25
#define ITERATIONS 12
#define NUM_PROCESSES 4

void printHistogram(int *hist) {
    for (int i = 0; i < HIST_SIZE; i++) {
        // printf("%2d: ", i - 12);  // Print the range from -12 to 12
        for (int j = 0; j < hist[i]; j= j + 1000) {
            printf("*");
        }
        printf("\n");
    }
}

void sampleAndFillHistogram(int samples, int *hist) {
    int counter , randomValue;
    for(int i=0 ; i < HIST_SIZE ; i++){
        hist[i] = 0;
    }
    for (int i = 0; i < samples; i++) {
        counter = 0;
        for (int j = 0; j < ITERATIONS; j++) {
            randomValue = rand() % 101;  // Generate random number between 0 and 100
            if (randomValue >= 49) {
                counter++;
            } else {
                counter--;
            }
        }
        if (counter >= -12 && counter < HIST_SIZE) {
            hist[counter + 12]++;  // Adjust index to fit -12 to +12 range into 0 to 24 index range
        }
        else{
            fprintf(stderr, "Error: counter out of bounds\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    srand(time(0));  // Seed the random number generator

    int shmid = shmget(IPC_PRIVATE, HIST_SIZE * sizeof(int), IPC_CREAT | 0666);
    int *hist = (int *)shmat(shmid, NULL, 0);
    for (int i = 0; i < HIST_SIZE; i++) hist[i] = 0;  // Initialize histogram

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    
    for (int p = 0; p < NUM_PROCESSES; p++) {
        pid_t pid = fork();
        if (pid == 0) {
            sampleAndFillHistogram(SAMPLE_SIZE_3 / NUM_PROCESSES, hist);
            shmdt(hist);
            exit(0);
        }
    }

    for (int p = 0; p < NUM_PROCESSES; p++) {
        wait(NULL);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for %d samples with %d processes: %f seconds\n", SAMPLE_SIZE_3, NUM_PROCESSES, cpu_time_used);

    printHistogram(hist);
    shmdt(hist);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}