#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
	clock_t t1, t2;				// variables for computing clocks 
	double a=1.234, b=2.456, c;
	double **A, *x, *b, T1, T2;
	int i, j, k, N=100000000;

	t1 = clock();

	t2 = clock();
	T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
	printf("mem x 3 + 1 loop:%f\n",T1);
	printf("(a,b)=%f %f\n", a,b);


	return 0;
} 
