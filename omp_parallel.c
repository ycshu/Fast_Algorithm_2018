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
	
	#pragma omp parallel num_threads(2) // 把下面的工作平行處理 
	{
		printf("Hello World A(%d,%d,%f)\n",omp_get_thread_num(),omp_get_num_threads(),omp_get_wtime()-ot1);
		printf("Hello World B(%d,%d,%f)\n",omp_get_thread_num(),omp_get_num_threads(),omp_get_wtime()-ot1);
	}
	// 從列印的結果，你會發現
	// 這兩行指令在同一個thread中是循序的，在不同的thread中先後不一定
	
	j = 0; 
	for(i=0;i<10;++i)
	{
		j += i;
	}
	printf("(without parallel) j = %d\n", j);
	
	j = 0;
	#pragma omp parallel for // by default, i is private, others is shared
	for(i=0;i<10;++i)
	{
		j += i;
		//printf("(thread_num, i, j)=(%d, %d, %d)\n", omp_get_thread_num(), i, j);
	}
	printf("(without any consideration) j = %d\n", j);
	/* 
	for example, we have 4 workers.

	shared	j=0	can be read and written by any workers below
		
				worker 1	worker 2	worker 3	worker 4
	private	i	  i=1		  i=2		  i=3		  i=4
	工作:		 j=j+i		 j=j+i		 j=j+i		 j=j+i
	執行順序	   1		   1		   2		   3	 (相同表幾乎同時)
	             j=0+1		 j=0+2 ==> 看哪一個最後送出去， j 就是那個值 
	*/
	
//=====================================================================================

	j = 0;
	#pragma omp parallel for 
	for(i=0;i<10;++i) 
	{
		#pragma omp atomic // 避免 j 的記憶體位置同時被兩個 thread 存取  通常比較慢 
		j += i;
	}
	printf("(atomic) j = %d\n", j);
	
//=====================================================================================
	
	j = 0;
	#pragma omp parallel for reduction(+: j) // = private j, sum all j in all thread 
	for(i=0;i<10;++i) 
	{
		j += i;
	}
	printf("(reduction) j = %d\n", j);

//=====================================================================================
	
	// sum 0~9
	int JG[10];
	j = 0;
	#pragma omp parallel num_threads(10) // 把下面的工作平行處理 (還是錯的) 
	{
		i = omp_get_thread_num();
		JG[i] = j;
		#pragma omp atomic   // i = 8, JG[8] = 22,        j += 0,... 
							 //                    i = 0,
		j += i;
	}
	printf("(+ thread number may be wrong) j = %d\n", j);

//=====================================================================================

	j = 0;
	#pragma omp parallel num_threads(10) private(i) // 把下面的工作平行處理
	{
		i = omp_get_thread_num();
		
		JG[i] = j;
		#pragma omp atomic   // i8 = 8, JG[8] = 22,         j += i8,... 
							 //                     i0 = 0, ....       j += i0
		j += i;
	}
	printf("(+ thread number correct) j = %d\n", j);	

//=====================================================================================

	j = 0;
	#pragma omp parallel num_threads(10)	//陽春型寫法 
	{
		#pragma omp atomic
		j += omp_get_thread_num();
	}
	printf("j = %d\n", j);
	
	return 0;

//=====================================================================================

	#pragma omp parallel num_threads(10) private(i)
	{
		i = omp_get_thread_num();
		printf("JG[%d]=%d,", i, JG[i]);
	}
	printf("\n");
	j = 0; 
	#pragma omp parallel num_threads(10)  // 把下面的工作平行處理 
	{
		i = omp_get_thread_num();
		#pragma omp barrier
		JG[i] = j;
		#pragma omp atomic
		j += i;
	}
	printf("j = %d\n", j);

//=====================================================================================

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

//=====================================================================================

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

//=====================================================================================

	#pragma omp parallel for private(k, j) // 分給 A=0, B=1, C=2, D, E, F, G, H 
										   // j也要是 private 的，不然 B 同學可能會拿到 A 同學的 j 
	for(k=0;k<100;++k) 
	{
		j = 0;
		#pragma omp parallel for reduction(+: j) // 分給 D_1, D_2, ..., D_10 
		for(i=0;i<=10;++i)
		{
			j += i;
			//printf("i=%d, j=%d, thread=%d\n",i,j,omp_get_thread_num());
		}
		//printf("sum(1..10) = %d\n",j);
		printf("%d thread\n", omp_get_thread_num());
		if(j != 55) 
		{
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
