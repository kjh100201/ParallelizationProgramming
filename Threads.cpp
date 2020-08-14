#include <stdio.h>
#include <chrono>
#include <algorithm>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#define N 1000
#define NUM_THREADS 8

double** A, ** B, ** C;

void* matrixMultiplication(void* t)
{
    int itemsPerThread = (N + NUM_THREADS - 1) / NUM_THREADS;
    int startIndex = *(int*)t - 1;
    startIndex = startIndex * itemsPerThread;
    int endIndex = std::min(startIndex + itemsPerThread, N);

	// Matrix Multiplication 
    for (int i = startIndex; i < endIndex; i++)
        for (int j = 0; j < N; j++)
        {
            double total = 0;
            for (int k = 0; k < N; k++)
                total += A[i][k] + B[k][j];
            C[i][j] = total;
        }

    return 0;
}

int main()
{
    A = new double* [N];
    B = new double* [N];
    C = new double* [N];

    srand((unsigned int)time(0));

    // Initialization Matrix
    for (int i = 0; i < N; i++)
    {
        A[i] = new double[N];
        B[i] = new double[N];
        C[i] = new double[N];

        for (int j = 0; j < N; j++)
        {
            A[i][j] = rand();
            B[i][j] = rand();
        }
    }

	// Starting Time 
    auto start_time = std::chrono::high_resolution_clock::now();

	// Setting Threads
    pthread_t pthreads[NUM_THREADS];

    for (int j = 0; j < NUM_THREADS; j++) {
        pthread_create(&pthreads[j], NULL, matrixMultiplication, &j);
    }

    for (int j = 0; j < NUM_THREADS; j++)
        pthread_join(pthreads[j], NULL);

	// Ending Time
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    printf("%f seconds\n", duration.count());

    return 0;
}