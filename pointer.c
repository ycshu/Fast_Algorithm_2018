#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
	int n;
	long long L, L1, L2; 
	float *f = &n;
	double *d = &L, *d1 = &L1, *d2 = &L2;
	
	n = 0x7F800000;
	L = 0x7FF0000000000000;
	L1 = 0x3FF0000000000000; 
	L2 = 0x3FF0000000000001;
	*d = (1.0/9);
	printf("%lld %.18e\n", L, *d);
	return;
}
