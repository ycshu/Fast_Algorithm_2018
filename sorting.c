#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#define DEBUG 0
int quicksort1(int *x, int left, int right);
int quicksort2(int *x, int left, int right);

int main()
{
	clock_t t1, t2;				// variables for computing clocks 
	int *x, *y, s, p;
	double T1;
	int i, j, N;

	srand( time(NULL) );

	for(N=40000;N<=40000;N*=2)
	{
		x = (int *) malloc( N * sizeof(int) );
		y = (int *) malloc( N * sizeof(int) );

		//#pragma omp parallel
		{
			//#pragma omp parallel for
			for(i=0;i<N;++i)
			{
				y[i] = x[i] = rand() % N;
			}
		}
		#if DEBUG
		for(i=0;i<N;++i)
		{
			printf("x[%d]=%d\n",i,x[i]);
		}
		#endif
		
		t1 = clock();
		for(i=0;i<N;++i) 
		{
			for(j=i+1;j<N;++j)
			{
				if(y[i]<y[j]) 
				{
					s = y[i];
					y[i] = y[j];
					y[j] = s;
				}
			}
		}
		
		t2 = clock();
		T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
		printf("Sorting %d elements: %f\n",N, T1);
		for(i=0;i<N;++i)
		{
			//printf("y[%d]=%f\n",i,y[i]);
		}
		
		for(i=0;i<N;++i) y[i] = x[i];
		
		t1 = clock();
		quicksort1(y,0,N);
		#if DEBUG
		for(i=0;i<N;++i)
		{
			printf("y[%d]=%d\n",i,y[i]);
		}
		#endif
		t2 = clock();
		T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
		printf("(1)Quick Sorting %d elements: %f\n",N, T1);

		for(i=0;i<N;++i) y[i] = x[i];
		
		t1 = clock();
		quicksort2(y,0,N);
		#if DEBUG
		for(i=0;i<N;++i)
		{
			printf("y[%d]=%d\n",i,y[i]);
		}
		#endif
		t2 = clock();
		T1 = (t2-t1)/(double) CLOCKS_PER_SEC;
		printf("(2)Quick Sorting %d elements: %f\n",N, T1);
		
		free(x);
		free(y);
	} 

	return 0;
}

int quicksort1(int *x, int left, int right)
{
	int i, j, k, pivot, N = right-left; 
	int *y;
	
	if(left < right-1)
	{
		y = (int *) malloc(N*sizeof(int));
		pivot = x[left+(rand() % N)];
		i = 0; j = N-1;
		for(k=0;k<N;++k) 
		{
			if(x[left+k] < pivot) 
			{
				y[i] = x[left+k];
				i = i + 1;
			}
			else
			{
				y[j] = x[left+k];
				j = j - 1;
			}
		}
		y[i] = pivot;
		#if DEBUG
		printf("%d %d %d %d %d\n",left,i,j,pivot,N);
		for(k=0;k<N;++k)
		{
			printf("y[%d]=%d\n",k,y[k]);
		}
		system("pause");
		#endif
		for(k=0;k<N;++k)
		{
			x[left+k] = y[k];
		}
		free(y);
		quicksort1(x,left,left+i);
		quicksort1(x,left+i+1,right);	
	}
	else
	{
		return 1;
	}
}


int quicksort2(int *x, int left, int right)
{
	int i, j, k;
	int pivot, t;
	
	//  
	if(left < right-1)
	{
		pivot = x[left];
    	i = left+1;
    	j = right-1;
    	// 
    	while(1)
		{
      		while(i < right && pivot >= x[i]) i++; 
      		while(j >  left && pivot <  x[j]) j--; 
      		#if DEBUG 
			printf("%d %d %d\n", i,j,pivot);
			#endif
      		if(i>=j) break;
      		t = x[i];
      		x[i] = x[j];
      		x[j] = t;
      		#if DEBUG
			for(k=left;k<right;++k)
			{
				printf("x[%d]=%d\n",k,x[k]);
			}
			system("pause");
			#endif
        }
        //t = x[left];
        x[left] = x[j];
        x[j] = pivot;
        #if DEBUG
        printf("i=%d,j=%d\n",i,j);
		for(k=left;k<right;++k)
		{
			printf("x[%d]=%d\n",k,x[k]);
		}
		system("pause");
        #endif
		quicksort2(x, left, j);
		quicksort2(x, j+1, right);
    }
    else 
    {
    	return 1;
	}
}
