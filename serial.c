#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLE_SIZE_1 5000
#define SAMPLE_SIZE_2 50000
#define SAMPLE_SIZE_3 500000
#define HIST_SIZE 25
#define ITERATIONS 12

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

    int hist[HIST_SIZE];  // Initialize histogram

    clock_t start, end;
    double cpu_time_used;

    // Run for SAMPLE_SIZE_1
    for (int i = 0; i < HIST_SIZE; i++) hist[i] = 0; // Reset histogram
    start = clock();
    sampleAndFillHistogram(SAMPLE_SIZE_1, hist);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for %d samples: %f seconds\n", SAMPLE_SIZE_1, cpu_time_used);
    printHistogram(hist);

    // Run for SAMPLE_SIZE_2
    for (int i = 0; i < HIST_SIZE; i++) hist[i] = 0; // Reset histogram
    start = clock();
    sampleAndFillHistogram(SAMPLE_SIZE_2, hist);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for %d samples: %f seconds\n", SAMPLE_SIZE_2, cpu_time_used);
    printHistogram(hist);

    // Run for SAMPLE_SIZE_3
    for (int i = 0; i < HIST_SIZE; i++) hist[i] = 0; // Reset histogram
    start = clock();
    sampleAndFillHistogram(SAMPLE_SIZE_3, hist);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for %d samples: %f seconds\n", SAMPLE_SIZE_3, cpu_time_used);
    printHistogram(hist);

    return 0;
}