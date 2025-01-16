Explanation of Code (Serial Version) 
File: serial.c

1. Initialization:  
   - Defines different sample sizes (SAMPLE_SIZE_1, SAMPLE_SIZE_2, SAMPLE_SIZE_3).  
   - Defines the histogram array size (HIST_SIZE).  
   - Initializes the `hist` array with zeros.

2. Function: sampleAndFillHistogram 
   - A `counter` variable is defined for each sample, starting from zero.  
   - For each sample, 12 random numbers between 0 and 100 are generated.  
   - If a random number is greater than or equal to 49, the counter is incremented; otherwise, it is decremented.  
   - Finally, the `counter` value is used as an index in the `hist` array, and that index's value is incremented.

3. Sampling and Time Measurement:  
   - For each sample size, the execution time of the sampling process is calculated and printed.  
   - The resulting histogram is then displayed.

Execution Time:  
| Sample Size   | 5000   | 50000  | 500000 |  
|---------------|--------|--------|--------|  
| Execution Time | 0.003 s | 0.014 s | 0.144 s |

---

Explanation of Code (Using fork())  
File: with_fork.c

1. Shared Memory:  
   - A shared memory section for the histogram array is allocated using `shmget`. Both parent and child processes use this shared memory to store results.

2. Creating Child Processes:  
   - Four child processes are created using `fork()`. Each child process handles part of the samples and stores the result in shared memory. The parent process waits for the child processes to finish (using `wait()`).

3. Race Condition:  
   - In this version of the code, child processes access the histogram array simultaneously, which may cause a race condition. This issue is addressed in the next version using semaphores.

Execution Time:  
| Sample Size   | 5000   | 50000  | 500000 |  
|---------------|--------|--------|--------|  
| Execution Time | 0.000189 s | 0.000222 s | 0.000444 s |

---

Does This Program Involve Race Condition?  
Yes.

Solution to Race Condition:  
In the second version of the code, the access to the `hist` array may lead to a race condition since multiple processes access it simultaneously. To prevent this issue, synchronization mechanisms like mutexes can be used. However, in this code, shared memory and independent processes are used, where each process operates on a separate section of the `hist` array, and the results are shared at the end. To implement mutex in a multi-process environment, semaphores are used. Below is the code for using semaphores to prevent race conditions.

---

Explanation of Code (Using fork() and Semaphores)  
File: fork_with_semaphore.c

1. Initialization:  
   - Similar to the serial code, but this time, shared memory is used for storing the histogram.  
   - A semaphore is created for synchronizing access to the histogram array.

2. Function: sampleAndFillHistogram  
   - Similar to the serial code, but this time access to the histogram array is synchronized using `sem_wait` and `sem_post`.

3. Creating Child Processes:  
   - Several child processes are created using `fork()`.  
   - Each child process processes part of the samples and stores the result in shared memory.  
   - The parent process waits for the child processes to finish (using `wait()`).

4. Time Measurement and Results:  
   - The execution time of the parallel sampling is calculated and printed.  
   - The final histogram is displayed.  
   - Semaphores and shared memory are freed.

---

Summary:  
- The serial code processes samples sequentially (single-threaded).  
- The parallel code performs sampling in parallel using child processes and semaphores for synchronization (multi-threaded).  
- Both codes calculate and report execution times for different sample sizes.
