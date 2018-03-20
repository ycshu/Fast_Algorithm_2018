#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

int main()
{
	int i, j, N=100000000;
	double a = 1.234, b = 2.345;
	double ot1, ot2;
	clock_t t1, t2;
	ot1 = omp_get_wtime();
	#pragma omp parallel num_threads(8)
	{
		printf("Hello world (%d,%d,%f) \n",omp_get_thread_num(),omp_get_num_threads(),omp_get_wtime()-ot1);
		printf("Hello Program\n");
	}
	
	#pragma omp parallel for 
	for(i=0;i<10;++i)
	{
		j = i;
		printf("%d %d\n",i,omp_get_thread_num());
	}
	printf("j = %d\n",j);
	
	j = 0;
	for(i=0;i<=10;++i)
	{
		j += i;
	}
	printf("sum(1..1000) = %d\n",j);

	j = 0;
	#pragma omp parallel for private(i)
	for(i=0;i<=1000;++i)
	{
		j += i;
	}
	printf("sum(1..10) = %d\n",j);
	
	
	t1 = clock();
	for(i=0;i<N;++i) {
		a = a*b;
		b = a/b;
		a = a/b;
	}
	t2 = clock();
	printf("a = %f, b = %f\n", a, b);
	printf("parallel time:%f\n",(t2-t1)/(double) CLOCKS_PER_SEC);
	
	t1 = clock();
	#pragma omp parallel for num_threads(4)
	for(i=0;i<N;++i) {
		a = a*b;
		b = a/b;
		a = a/b;
	}
	t2 = clock();
	printf("a = %f, b = %f\n", a, b);
	printf("parallel time:%f\n",(t2-t1)/(double) CLOCKS_PER_SEC);
	
	return 0;
}
