#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#define DEBUG 0

int main()
{
	int    k, n, N = 1 << 15;  // (00000001)=1  (00001000)=8
	double ca, sa, a, theta, c, s, t, *X_re, *X_im, *x_re, *x_im;
	clock_t t1, t2;
	
	x_re = (double *) malloc(N*sizeof(double));
	x_im = (double *) malloc(N*sizeof(double));
	X_re = (double *) malloc(N*sizeof(double));
	X_im = (double *) malloc(N*sizeof(double));
	
	for(n=0;n<N;++n)
	{
		x_re[n] = n+1;
		x_im[n] = 0.0;
	}
	
	t1 = clock();
	for(k=0;k<N;++k)
	{
		X_re[k] = 0.0;
		X_im[k] = 0.0;
		//   cos(2*PI*k*n/N) n = 0, 1, 2, 3...
		//   theta = n * (2*PI*k/N) = n * a
		//   cos(n a) -> cos((n+1)a) = cos(na) cos(a) - sin(na) sin(a)
		//   sin(n a) -> sin((n+1)a) = sin(na) cos(b) + cos(na) sin(b)
		a  = 2*M_PI*k/N;
		ca = cos(a);
		sa = sin(a);
		c  = 1.0;
		s  = 0.0;
		for(n=0;n<N;++n)
		{
			// x_n = x_re + i * x_im
			// x_n (cos(...)-i sin(...)
			// real: x_re * cos(...) + x_im * sin(...)
			// imag: x_im * cos(...) - x_re * sin(...)
			// theta = 2*M_PI*k*n/N;
			// c = cos(theta) = cos(na);
			// s = sin(theta) = sin(na);
			X_re[k] += x_re[n]*c + x_im[n]*s;
			X_im[k] += x_im[n]*c - x_re[n]*s;
			//   cos((n+1)a) = cos(na) cos(a) - sin(na) sin(a)
			//   sin((n+1)a) = sin(na) cos(a) + cos(na) sin(a)
			t =  c;
			c =  c*ca - s*sa;
			s =  s*ca + t*sa; 
		}
	}
	t2 = clock();
	
	printf("N = %d, Time: %f\n", N, 1.0*(t2-t1)/CLOCKS_PER_SEC);
	
	#if DEBUG
	for(k=0;k<N;++k)
	{
		if(X_im[k] >=0 )
			printf("%f + %f i\n", X_re[k], X_im[k]);
		else
			printf("%f - %f i\n", X_re[k], -X_im[k]);
			
	}
	#endif
	return 0;
} 
