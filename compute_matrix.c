#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
	clock_t t1, t2;				// variables for computing clocks 
	double **A, *x, *b, T1, s;
	int i, j, N=10000;

	srand(time(NULL));

	for(N=2000;N<=20000;N*=2)
	{
		A = (double **) malloc( N * sizeof(double*) );
		A[0] = (double *) malloc( N*N*sizeof(double));
		#pragma omp parallel for 
		for(i=1;i<N;++i) A[i] = A[0] + i*N;
		x = (double *) malloc( N * sizeof(double) );
		b = (double *) malloc( N * sizeof(double) );
		
		#pragma omp parallel for
		for(i=0;i<N;++i)
		{
			x[i] = 1.0*rand()/RAND_MAX;
			#pragma omp parallel for
			for(j=0;j<N;++j)
			{
				A[i][j] = 1.0*rand()/RAND_MAX;
			}
		}

		t1 = clock();
		for(i=0;i<N;++i) 
		{
			s = 0.0;
			for(j=0;j<N;++j)
			{
				s += A[i][j]*x[j];
			}
			b[i] = s;
		}
		t2 = clock();
		T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
		printf("Matrix time vector (not parallel) N=%d : %f\n",N, T1);
		
		t1 = clock();
		#pragma omp parallel for private(i,s) num_threads(1)
		for(i=0;i<N;++i) 
		{
			s = 0.0;
			#pragma omp parallel for reduction(+: s)
			for(j=0;j<N;++j)
			{
				s += A[i][j]*x[j];
			}
			b[i] = s;
		}
		t2 = clock();
		T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
		printf("Matrix time vector for N=%d : %f\n",N, T1);
		free(b);
		free(x);
		free(A[0]);
		free(A);	
	} 

	return 0;
} 
