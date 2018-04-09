#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
	clock_t t1, t2;				// variables for computing clocks 
	double a=1.234, b=2.456, c;
	double T1, T2;
	int i, j, k, N=100000000;
	t1 = clock();
	for(i=0;i<N;++i)
	{
		c = b;
		b = a;
		a = c;
	}
	t2 = clock();
	T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("mem x 3 + 1 loop:%f\n",T1);
	printf("(a, b, c)=%f %f %f\n", a, b, c);

	t1 = clock();
	for(i=0;i<N;++i)
	{
		c = b;
		b = a;
		a = c;
		c = b;
		b = a;
		a = c;
	}
	t2 = clock();
	T2 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("mem x 6 + 1 loop:%f\n",T2);
	printf("mem:%f\n",(T2-T1)/3.0);
	printf("(a,b,c)=%f %f %f\n", a, b,c);
	
	printf("====================================\n");
		
	//以下為 a 與 b 互換的程式 (計算加法所需的時間) 
	t1 = clock();
	for(i=0;i<N;++i)
	{
		a = a+b;
		b = a-b;
		a = a-b;
	}
	t2 = clock();
	T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("(+,-) time x 3 + 1 loop:%f\n",T1);
	printf("(a,b)=%f %f\n", a,b);
	t1 = clock();
	
	#pragma omp parallel	//平行計算 
	for(i=0;i<N;++i)
	{
		a = a+b;
		b = a-b;
		a = a-b;
		a = a+b;
		b = a-b;
		a = a-b;
	}
	t2 = clock();
	T2 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("(+,-) time x 6 + 1 loop:%f\n",T2);
	printf("Real (+,-) time: %f\n",(T2-T1)/3.0);	
	printf("(a,b)=%f %f\n", a,b);
	
	printf("====================================\n");
	
	//以下為 a 與 b 互換的程式 (計算乘法所需的時間) 
	t1 = clock();
	for(i=0;i<N;++i)
	{
		a = a*b;
		b = a/b;
		a = a/b;
	}
	t2 = clock();
	T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("(*,/) time x 3 + 1 loop:%f\n",T1);
	printf("(a,b)=%f %f\n", a,b);

	t1 = clock();
	for(i=0;i<N;++i)
	{
		a = a*b;
		b = a/b;
		a = a/b;
		a = a*b;
		b = a/b;
		a = a/b;
	}
	t2 = clock();
	T2 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("(*,/) time x 6 + 1 loop:%f\n",T2);
	printf("(*,/) time:%f\n",(T2-T1)/3.0);	
	printf("(a,b)=%f %f\n", a,b);

	printf("====================================\n");
	
	//以下為計算 sin 時間的程式 
	
	t1 = clock();
	for(i=0;i<N;++i)
	{
		a = sin(a);
	}
	t2 = clock();
	T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("(sin) time + 1 loop:%f\n",T1);
	printf("(a,b)=%f %f\n", a,b);

	t1 = clock();
	for(i=0;i<N;++i)
	{
		a = sin(b);
		b = sin(a);
	}
	t2 = clock();
	T2 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("(sin) time x 2 + 1 loop:%f\n",T2);
	printf("(sin) time: %f\n",T2-T1);
	printf("(a,b)=%.16e %.16e\n", a,b);

	printf("====================================\n");
	
	//以下為計算 "隨機取亂數" 所需時間的程式 
	srand(time(NULL));
	short r;

	//有平行...花的時間反而比較多?! 
	#pragma omp parallel for 
	t1 = clock();
	for(i=0;i<N;i++)
	{
		r = rand();
	}
	t2 = clock();
	printf("(rand) parallel time:%f\n",(t2-t1)/(double) CLOCKS_PER_SEC);
	printf("r=%d\n", r);
	
	//未平行 
	t1 = clock();
	for(i=0;i<N;i++)
	{
		r = rand();
	}
	t2 = clock();
	printf("(rand) time:%f\n",(t2-t1)/(double) CLOCKS_PER_SEC);
	printf("r=%d\n", r);
	return;
} 
