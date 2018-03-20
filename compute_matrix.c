#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
	clock_t t1, t2;				// variables for computing clocks 
	double **A, *x, *b, T1;
	int i, j, N=10000;

	srand(time(NULL));

	for(N=2000;N<=20000;N*=2)
	{
	
	A = (double **) malloc( N * sizeof(double*) );
	A[0] = (double *) malloc( N*N*sizeof(double));
	for(i=1;i<N;++i) A[i] = A[i-1] + N;
	x = (double *) malloc( N * sizeof(double) );
	b = (double *) malloc( N * sizeof(double) );
	
	for(i=0;i<N;++i)
	{
		for(j=0;j<N;++j)
		{
			A[i][j] = rand();
		}
		x[i] = rand();
	}
	t1 = clock();
	for(i=0;i<N;++i) 
	{
		b[i] = 0.0;
		for(j=0;j<N;++j)
		{
			b[i] += A[i][j]*x[j];
		}
	}
	t2 = clock();
	T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("Matrix time vector :%f\n",T1);
	free(b);
	free(x);
	free(A[0]);
	free(A);
	
	} 

	return 0;
} 
