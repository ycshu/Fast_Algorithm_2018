#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main()
{
	clock_t t1, t2;				// variables for computing clocks 
	double *x, *b, *c, T1, T2;		// x = 1234, a location in memory, x[0] the value at memory 1234, 1235, 1236, ... 1241
	double **A;
	int i, j, k, L, M, N=10000;		
		
	for(N=18384;N<=18384;N*=2)
	{												   
		A = (double **) malloc( N * sizeof(double *));  
		A[0] = (double *) malloc(N*N*sizeof(double));

		#pragma omp parallel for
		for(i=1;i<N;++i) 
		{
			A[i] = A[0] + i*N;
		}
		// A[1] = A[0] + N, A[2] = A[0]+2N, A[3] = A[0]+3N ==> 可以平行處理 

		x = (double *) malloc( N * sizeof(double));
		b = (double *) malloc( N * sizeof(double));
		c = (double *) malloc( N * sizeof(double));
		
		//平行造出random的 A 矩陣 與 x 向量 
		M = N/4;
		#pragma omp parallel num_threads(4) private(i,j,k,L)
		{
			k = omp_get_thread_num();
			//printf("the seed at thread %d is : %d\n",k,time(NULL)>>k);
			srand(time(NULL)>>k);			// 在每一個 thread 中設定起始值 
			for(i=k*M;i<(k+1)*M;++i)
			{
				for(j=0;j<N;++j)
				{
					A[i][j] = rand() % 10;
				}
				x[i] = rand();
			}
		}
		
/*
		//將A矩陣印出來 
		printf("A = \n");
		for(i=0;i<N;++i)
		{
			for(j=0;j<N;++j)
			{
				printf("%d ",A[i][j]);
			}
			printf("\n");
		}
		
		printf("============================================\n");
		
		//將x向量印出來 
		printf("x = \n");
		for(i=0;i<N;++i)
		{
			printf("%f \n",x[i]);
		}
*/		
		// for correct answer
		double t;
		t1 = clock();
		for(i=0;i<N;++i) 
		{
			t = 0.0;
			for(j=0;j<N;++j)
			{
				t += A[i][j]*x[j];
			}
			b[i] = t;
		}
		t2 = clock();
		T1 = (t2-t1)/(double)CLOCKS_PER_SEC;

/*		
		printf("============================================\n");
		
		printf("for correct answer b = \n");
		for(i=0;i<N;++i) 
		{
			printf("%f \n",b[i]);
		}
		
		printf("============================================\n");
*/
		
		// for parallel answer
		t1 = clock();
		M = N/4;
		#pragma omp parallel num_threads(4) private(i,j,k,t)
		{
			k = omp_get_thread_num();
			for(i=k*M;i<(k+1)*M;++i) 
			{
				t = 0.0;
				for(j=0;j<N;++j)
				{
					t += A[i][j]*x[j];
				}
				c[i] = t;
			}
		}
		t2 = clock();
		T2 = (t2-t1)/(double) CLOCKS_PER_SEC;

/*		
		printf("============================================\n");
		
		printf("for parallel answer b = \n");
		for(i=0;i<N;++i) 
		{
			printf("%f \n",c[i]);
		}
*/
			
		for(i=0;i<N;++i)
		{
			if(fabs(b[i]-c[i])!=0)
			printf("Wrong at i = %d, the fabs(b[i]-c[i]) = %f\n", i, fabs(b[i]-c[i]));
		}
		printf("Matrix time vector : %f\n",T1);
		printf("Matrix time vector (parallel) : %f\n",T2);
		free(b);
		free(x);
		free(A[0]);
		free(A);
	} 

	return 0;
} 
