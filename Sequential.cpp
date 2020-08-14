#include <stdio.h>
#include <stdlib.h>
#include <chrono>

int N = 1000;
double** A, ** B, ** C;

int main(int argc, char* argv[])
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

	// Matrix Multiplication 
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			int total = 0;
			for (int k = 0; k < N; k++)
				total += A[i][k] + B[k][j];
			C[i][j] = total;
		}

	// Ending Time
	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end_time - start_time;
	printf("%f seconds\n", duration.count());

	return 0;
}
