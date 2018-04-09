#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 
#include <time.h>

int main()
{
	clock_t t1, t2;				// variables for computing clocks 

	double **A, *x, *b, *c, T1;		// x = 1234, a location in memory, x[0] the value at memory 1234, 1235, 1236, ... 1041
	int **C;
	int i, j, k, L, M, N=10000;		//
	
	T1 = 1.2;
	x = &T1;
	printf("%u %f\n", x, x[0]);

	//以下此五個亂數每次都跑出相同的值 
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("-------------\n");

	srand(time(NULL)); // 設定亂數種子為現在的時間  以下的五個就會隨時間變成不同的亂數 
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("-------------\n");

	for(N=16;N<=16;N*=2)
	{												   // N=4, sizeof(double*)=4, 16 bytes
													   // 1234(00), 1235(00), 1236(04), 1237(00)... 
													   // A = 1234
													   // A[0] = (00000400)_HEX = 1024, A[1] = 1238..1241
													   // A[0][0],   memory 1024, 1025, 1026 ,.... 1031 => double 8 bytes
													   // A[0][1],   memory 1032, 1033, 1034 ,.... 1039
		A = (double **) malloc( N * sizeof(double*) ); // 在記憶體中拿N個double*記憶體, 
													   // 並且把一開始指標放在A, A[0], A[1], A[2], A[N-1]
													   // 但是  A[0], A[1], A[2], A[N-1] 的值是未給定的 
		A[0] = (double *) malloc( N*N*sizeof(double)); // 在記憶體中拿N*N個double記憶體, 
													   // 並且把一開始指標放在A[0] 
													   // A[0][0], A[0][1], .... A[0][N*N-1] 尚未給定 
		for(i=1;i<N;++i) A[i] = A[i-1] + N;			   // A[1] = A[0]+N ==> A[1][0] = A[0][N]
													   // A[2] = A[1]+N = A[0]+2N ==> A[2][0] = A[0][2N]
													   // case: N = 3
													   // A[0][0] A[0][1] A[0][2]
													   // A[1][0] A[1][1] A[1][2] ==> A[0][3], A[0][4], A[0][5]
													   // A[2][0] A[2][1] A[2][2] ==> A[0][6], A[0][7], A[0][8] 
		// A[1] = A[0] + N --> A[2]=A[1]+N --> A[3]=A[2]+N 不可能平行運算的
		
		#pragma omp parallel for
		for(i=1;i<N;++i) A[i] = A[0] + i*N;
		// A[1] = A[0] + N, A[2] = A[0]+2N, A[3] = A[0]+3N ==> 可以平行處理 


		x = (double *) malloc( N * sizeof(double) );
		b = (double *) malloc( N * sizeof(double) );
		c = (double *) malloc( N * sizeof(double) );
		C = (int **) malloc(N*sizeof(int*));
		C[0] = (int *) malloc(N*N*sizeof(int));
		
		/*
		#pragma omp parallel
		{
			srand(time(NULL));			//在每一個thread中設定起始值 
			#pragma omp parallel for	//再往下做取亂數 
			for(i=0;i<N;++i)
			{
				#pragma omp parallel for
				for(j=0;j<N;++j)
				{
					A[i][j]=rand();
				}
				x[i]=rand;
			}
		}
		*/
		 
		for(i=1;i<N;++i) C[i] = C[0]+i*N; 

		M = N/4;
		#pragma omp parallel num_threads(4) private(i,j,k,L)
		{
			k = omp_get_thread_num();
			printf("the seed at thread %d is : %d\n",k,time(NULL)>>k);
			srand(time(NULL)>>k);			// 在每一個 thread 中設定起始值 
			//#pragma omp parallel for // 再往下做取亂數 
			for(i=k*M;i<(k+1)*M;++i)
			{
				//L = omp_get_thread_num();
				//printf("thread %d, %d\n",k,L);
				for(j=0;j<N;++j)
				{
					A[i][j] = rand();
					C[i][j] = rand() % 10;
				}
				x[i] = rand();
			}
		}
		for(i=0;i<N;++i) 
		{ 
			for(j=0;j<N;++j)
			{
				printf("%d ",C[i][j]);
			}
			printf("\n");
		}
		
		// for correct answer
		double t;
		for(i=0;i<N;++i) 
		{
			t = 0.0;
			for(j=0;j<N;++j)
			{
				t += A[i][j]*x[j];
			}
			b[i] = t;
		}
		
		t1 = clock();
		#pragma omp parallel for private(i,j,t)
		for(i=0;i<N;++i) 
		{
			k = omp_get_thread_num();
			t = 0.0;
			#pragma omp parallel for 
			for(j=0;j<N;++j)
			{
				L = omp_get_thread_num();
				printf("%d %d\n",k,L);
				t += A[i][j]*x[j];
			}
			c[i] = t;
		}
		t2 = clock();
		T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
		for(i=0;i<N;++i)
		{
			printf("%f\n",fabs(b[i]-c[i]));
		}
		printf("Matrix time vector :%f\n",T1);
		
		free(b);
		free(x);
		free(A[0]);
		free(A);	
	} 

	return 0;
} 
