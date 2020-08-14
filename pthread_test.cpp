// Matrix_parallel_multiple.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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


// ������ �Լ�
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
	
	//��� ũ�� ���� �Ҵ� �κ�
	for(i = 0 ; i < n ; i++){
		a[i] = (int *)malloc(n * sizeof(int));
		b[i] = (int *)malloc(n * sizeof(int));
		r[i] = (int *)malloc(n * sizeof(int));
	}
	
	
	//��� �ʱ�ȭ �κ�
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			a[i][j]=rand()%10;
			b[i][j]=rand()%9;
			r[i][j] = 0;
		}
	}

	//��� ���� ��� �κ�
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			for(p = 0 ; p < n ; p++){
				r[i][j] += a[i][p] * b[p][j];
			}
		}
	}

/*��� ��� �κ�
	printf("��� A\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("\n��� B\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	printf("\n��� C\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", r[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
*/


	//�ð� ��ºκ�
	QueryPerformanceCounter(&end);
	double Time = ((double)end.QuadPart - (double)start.QuadPart)/((double)freq.QuadPart);
	printf("�� ����ó�� : %lf\n", Time);


	/* ���� ó�� */
	//��� �ʱ�ȭ �κ�
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			a[i][j]=rand()%10;
			b[i][j]=rand()%9;
			r[i][j] = 0;
		}
	}
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	//��� ���� ����ó�� ��� �κ�
#pragma omp parallel for private(p,j)
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			for(p = 0 ; p < n ; p++){
				r[i][j] += a[i][p] * b[p][j];
			}
		}
	}

/*��� ��� �κ�
	printf("��� A\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("\n��� B\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	printf("\n��� C\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", r[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
*/

	//�ð� ��ºκ�
	QueryPerformanceCounter(&end);
	double Parallel_Time = ((double)end.QuadPart - (double)start.QuadPart)/((double)freq.QuadPart);
	printf("����ó��(OpenMP) : %lf\n", Parallel_Time);


	//printf("�� ����ó�� �ð� - ����ó�� �ð� = %lf\n", Time-Parallel_Time);

	/* ���� ó�� */
	//��� �ʱ�ȭ �κ�
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			a[i][j]=rand()%10;
			b[i][j]=rand()%9;
			r[i][j] = 0;
		}
	}


    // ������ ���� �ƱԸ�Ʈ�� 1 �� �ѱ��.  
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    // ������ ���� �ƱԸ�Ʈ�� 2 �� �ѱ��. 
    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&b);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }


	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	// ������ ���Ḧ ��ٸ���. 
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
/*��� ��� �κ�
	printf("��� A\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("\n��� B\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	printf("\n��� C\n");
	for(i = 0 ; i < n ; i++){
		for(j = 0 ; j < n ; j++){
			printf("%d ", r[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
*/

	//�ð� ��ºκ�
	QueryPerformanceCounter(&end);
	Parallel_Time = ((double)end.QuadPart - (double)start.QuadPart)/((double)freq.QuadPart);
	printf("����ó��(Pthread) : %lf\n", Parallel_Time);
	

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

