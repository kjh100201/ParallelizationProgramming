// Matrix_parallel_multiple.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <omp.h>

#include <pthread.h>


int n = 1000;
int **a, **b, **r;


// 쓰레드 함수
void *t_function(void *data)
{
    int id;
    int i, j, p;
    id = *((int *)data);
	
	

	if(id == 1)
	{
		for(i = 0 ; i < n/2 ; i++){
			for(j = 0 ; j < n ; j++){
				for(p = 0 ; p < n ; p++){
					r[i][j] += a[i][p] * b[p][j];
				}
			}
		}
	}
	else
	{
	
		for(i = n/2 ; i < n ; i++){
			for(j = 0 ; j < n ; j++){
				for(p = 0 ; p < n ; p++){
					r[i][j] += a[i][p] * b[p][j];
				}
			}
		}
		
	}
return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	
	int i, j, p;
	pthread_t p_thread[2];
    int thr_id;
    int status;
    int a1 = 1;
    int b1 = 2;
	
	a = (int **)malloc(n * sizeof(int *));
	b = (int **)malloc(n * sizeof(int *));
	r = (int **)malloc(n * sizeof(int *));
	srand(time(NULL));
	LARGE_INTEGER freq, start, end;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	
	//행렬 크기 동적 할당 부분
	for(i = 0 ; i < n ; i++){
		a[i] = (int *)malloc(n * sizeof(int));
		b[i] = (int *)malloc(n * sizeof(int));
		r[i] = (int *)malloc(n * sizeof(int));
	}
	
	
	//행렬 초기화 부분
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			a[i][j]=rand()%10;
			b[i][j]=rand()%9;
			r[i][j] = 0;
		}
	}

	//행렬 곱셈 계산 부분
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			for(p = 0 ; p < n ; p++){
				r[i][j] += a[i][p] * b[p][j];
			}
		}
	}

/*행렬 출력 부분
	printf("행렬 A\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("\n행렬 B\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	printf("\n행렬 C\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", r[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
*/


	//시간 출력부분
	QueryPerformanceCounter(&end);
	double Time = ((double)end.QuadPart - (double)start.QuadPart)/((double)freq.QuadPart);
	printf("비 병렬처리 : %lf\n", Time);


	/* 병렬 처리 */
	//행렬 초기화 부분
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			a[i][j]=rand()%10;
			b[i][j]=rand()%9;
			r[i][j] = 0;
		}
	}
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	//행렬 곱셈 병렬처리 계산 부분
#pragma omp parallel for private(p,j)
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			for(p = 0 ; p < n ; p++){
				r[i][j] += a[i][p] * b[p][j];
			}
		}
	}

/*행렬 출력 부분
	printf("행렬 A\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("\n행렬 B\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	printf("\n행렬 C\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", r[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
*/

	//시간 출력부분
	QueryPerformanceCounter(&end);
	double Parallel_Time = ((double)end.QuadPart - (double)start.QuadPart)/((double)freq.QuadPart);
	printf("병렬처리(OpenMP) : %lf\n", Parallel_Time);


	//printf("비 병렬처리 시간 - 병렬처리 시간 = %lf\n", Time-Parallel_Time);

	/* 병렬 처리 */
	//행렬 초기화 부분
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			a[i][j]=rand()%10;
			b[i][j]=rand()%9;
			r[i][j] = 0;
		}
	}


    // 쓰레드 생성 아규먼트로 1 을 넘긴다.  
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    // 쓰레드 생성 아규먼트로 2 를 넘긴다. 
    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&b);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }


	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	// 쓰레드 종료를 기다린다. 
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
/*행렬 출력 부분
	printf("행렬 A\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("\n행렬 B\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	printf("\n행렬 C\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", r[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
*/

	//시간 출력부분
	QueryPerformanceCounter(&end);
	Parallel_Time = ((double)end.QuadPart - (double)start.QuadPart)/((double)freq.QuadPart);
	printf("병렬처리(Pthread) : %lf\n", Parallel_Time);
	

	for(i = 0 ; i < n ; i++){
		free(a[i]);
		free(b[i]);
		free(r[i]);
	}
	free(a);
	free(b);
	free(r);

	return 0;
}

