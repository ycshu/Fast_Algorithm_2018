#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

int main()
{
	int i, j, k, N=100000000;
	double a = 1.234, b = 2.345;
	double ot1, ot2;
	clock_t t1, t2;
	ot1 = omp_get_wtime();
	
	#pragma omp parallel num_threads(8) // 把下面的工作平行處理 
	{
		//printf("Hello World (%d,%d,%f) \n",omp_get_thread_num(),omp_get_num_threads(),omp_get_wtime()-ot1);
		//printf("Hello Program (%d,%d,%f)\n",omp_get_thread_num(),omp_get_num_threads(),omp_get_wtime()-ot1);
	}
	
	#pragma omp parallel for 
	for(i=0;i<10;++i)
	{
		j = i;
		// printf("%d %d\n",i,omp_get_thread_num());
	}
	// printf("j = %d\n",j);
	
	t1 = clock(); 
	j = 0;
	for(i=0;i<=10;++i)
	{
		j += i;
		//printf("%d %d\n",i);
	}
	printf("sum(1..10) = %d\n",j);

	for(k=0;k<100;++k)
	{
		j = 0;
		#pragma omp parallel for
		for(i=0;i<=10;++i)
		{
			j += i;
			//printf("i=%d, j=%d, thread=%d\n",i,j,omp_get_thread_num());
		}
		//printf("sum(1..10) = %d\n",j); // 這樣算總和有時會GG
		if(j != 55)
		{
			//printf("Wrong! %d\n",k);
		}
	}

	#pragma omp parallel for private(k, j) 
	for(k=0;k<100;++k) {
		j = 0;
		#pragma omp parallel for reduction(+: j)
		for(i=0;i<=10;++i)
		{
			j += i;
			//printf("i=%d, j=%d, thread=%d\n",i,j,omp_get_thread_num());
		}
		//printf("sum(1..10) = %d\n",j);
		printf("%d thread\n", omp_get_thread_num());
		if(j != 55) {
			printf("The summation is Wrong! %d\n", k);
		}
	}
	
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
