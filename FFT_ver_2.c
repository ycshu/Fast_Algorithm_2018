#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

int main()
{
	int i;
	double y_re[8], y_im[8], x_re[8], x_im[8];
	for(i=0;i<8;++i)
	{
		x_re[i] = i;
		x_im[i] = 0.0;
	}
	bit_reverse(x_re, x_im, 8);	
	butterfly(x_re, x_im, 8);
	
	for(i=0;i<8;++i)
	{
		printf("%f + %f i\n", x_re[i], x_im[i]);
	}
	return;
	 
}

int bit_reverse(double *x_re, double *x_im, int N)
{
    int m,p,q,k;
    double t;
    
    m = N/2;                        // Bit-Reverse 每次要進位的數字 
    q = m;							// p = 1, q = m (第一個要交換的) 
    for(p=1;p<N-1;++p)
    {
        //printf("%d <-> %d\n", p,q);
        if(p < q)
        {
            t = x_re[p];
            x_re[p] = x_re[q];
			x_re[q] = t;
            t = x_im[p];
            x_im[p] = x_im[q];
			x_im[q] = t;			 
        }
        k = m;						// k, 用來檢查第 log_2 k + 1 位是不是1 
        							// (1110) = 14 >= (8=k)
									// (0110) = 6  >= (4=k)
									// (0010) = 2  >= (2=k)
									// (0000) = 0  >= (1=k) X break --> (0001)
									// (0110) = 6  >= (8=k) X break --> (1110) 
        while(q >= k & k > 0)		// q >=k 第 (log_2 k + 1)位是1,  
        {
            q = q-k;				// 1->0
            k = k/2;				// 檢查下一位 
        }
        q = q+k;
    }
    return 0;
}
int butterfly(double *x_re, double *x_im, int N)
{
	int k, p, q, m;
	double w_re, w_im, w_N_re, w_N_im, t; 
	m = 1;
	while(m<N)
	{
		w_re = 1.0;
		w_im = 0.0; 		
		w_N_re =  cos(M_PI/m);
		w_N_im = -sin(M_PI/m);
		for(k=0;k<m;++k) 
		{
			for(p=k;p<N;p+=2*m)
			{
				q = p + m;
				// printf("(%d,%d) (%f,%f) FFT2 \n", p,q, w_re, w_im);
				// multiply (w_re + w_im * i) on x[q]
				t = x_re[q]; 
				x_re[q] = w_re*x_re[q] - w_im*x_im[q];
				x_im[q] = w_re*x_im[q] + w_im*t; 
				
				t = x_re[p];
				x_re[p] = x_re[p] + x_re[q];
				x_re[q] = t       - x_re[q]; 
				t = x_im[p];
				x_im[p] = x_im[p] + x_im[q];
				x_im[q] = t       - x_im[q]; 
				//
				
			}
			t    = w_re; 
			w_re = w_N_re*w_re - w_N_im*w_im;
			w_im = w_N_re*w_im + w_N_im*t;
		}
		m = m * 2;
	}
	
	return;
}



int Fast_Fourier_Transform(double *y_re, double *y_im, double *x_re, double *x_im, int N)
{
	if(N==2) 
	{
		// y, y[0] = x[0]+x[1], y[1] = x[0] - x[1]
		y_re[0] = x_re[0] + x_re[1];
		y_im[0] = x_im[0] + x_im[1];
		y_re[1] = x_re[0] - x_re[1]; 
		y_im[1] = x_im[0] - x_im[1];
	} else 
	{
		int k;
		double *y_even_re, *y_even_im, *y_odd_re, *y_odd_im;
		double *x_even_re, *x_even_im, *x_odd_re, *x_odd_im;
		double w_re, w_im, w_N_re, w_N_im, a, b, temp;
		y_even_re = (double *) malloc( N/2 * sizeof(double));
		y_even_im = (double *) malloc( N/2 * sizeof(double));
		x_even_re = (double *) malloc( N/2 * sizeof(double));
		x_even_im = (double *) malloc( N/2 * sizeof(double));
		y_odd_re = (double *) malloc( N/2 * sizeof(double));
		y_odd_im = (double *) malloc( N/2 * sizeof(double));
		x_odd_re = (double *) malloc( N/2 * sizeof(double));
		x_odd_im = (double *) malloc( N/2 * sizeof(double));
		for(k=0;k<N/2;++k)
		{
			x_even_re[k] = x_re[2*k];
			x_even_im[k] = x_im[2*k];
			x_odd_re[k]  = x_re[2*k+1];
			x_odd_im[k]  = x_im[2*k+1];
		}
		Fast_Fourier_Transform(y_even_re, y_even_im, x_even_re, x_even_im, N/2);
		Fast_Fourier_Transform(y_odd_re, y_odd_im, x_odd_re, x_odd_im, N/2);
		// y_k = even_k + w_N^k odd_k = even_k + (a + bi)
		w_N_re =  cos(2.0*M_PI/N);
		w_N_im = -sin(2.0*M_PI/N);
		w_re   = 1.0;
		w_im   = 0.0; 
		for(k=0;k<N/2;++k)
		{
			a = w_re*y_odd_re[k] - w_im*y_odd_im[k];
			b = w_re*y_odd_im[k] + w_im*y_odd_re[k];
			y_re[k]     = y_even_re[k] + a;
			y_im[k]     = y_even_im[k] + b;
			y_re[N/2+k] = y_even_re[k] - a;
			y_im[N/2+k] = y_even_im[k] - b;
			temp = w_re;
			w_re = w_re*w_N_re - w_im*w_N_im;
			w_im = temp*w_N_im + w_im*w_N_re;
		}
		free(y_even_re);
		free(x_even_re);
		free(y_even_im);
		free(x_even_im);
		free(y_odd_re);
		free(y_odd_im);
		free(x_odd_re);
		free(x_odd_im);
	}
}
