#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <mpi.h>

#define N 1000

MPI_Status status;
double A[N][N], B[N][N],C[N][N];

int main(int argc, char* argv[])
{
    srand((unsigned int)time(0));

    // Initialization Matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = rand();
            B[i][j] = rand();
        }
    }

    int NUM_TOTAL, coreID, NUM_CORE, source, receiverID, itemPer, offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &coreID);
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_TOTAL);
    NUM_CORE = NUM_TOTAL - 1;

    if (coreID == 0) {
		// Starting Time 
		auto start_time = std::chrono::high_resolution_clock::now();
		
        itemPer = N / NUM_CORE;
        offset = 0;

		// Setting Message Passing
        for (receiverID = 1; receiverID <= NUM_CORE; receiverID++)
        {
            MPI_Send(&offset, 1, MPI_INT, receiverID, 1, MPI_COMM_WORLD);
            MPI_Send(&itemPer, 1, MPI_INT, receiverID, 1, MPI_COMM_WORLD);
            MPI_Send(&A[offset][0], itemPer * N, MPI_DOUBLE, receiverID, 1, MPI_COMM_WORLD);
            MPI_Send(&B, N * N, MPI_DOUBLE, receiverID, 1, MPI_COMM_WORLD);
            offset = offset + itemPer;
        }

        for (int i = 1; i <= NUM_CORE; i++)
        {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&itemPer, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], itemPer * N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
        }
		
		// Ending Time
		auto end_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end_time - start_time;
		printf("%f seconds\n", duration.count());
    }

	// Actual Workers
    if (coreID > 0) {
        source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&itemPer, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&A, itemPer * N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, N * N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

		// Matrix Multiplication 
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
            {
                double total = 0;
                for (int k = 0; k < N; k++)
                    total += A[i][k] + B[k][j];
                C[i][j] = total;
            }


        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&itemPer, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&C, itemPer * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}