#include <stdio.h>
#include <stdlib.h>
//#include <omp.h>
#include <math.h>
#include <time.h>
int quicksort(int *x, int left, int right);

int main()
{
	clock_t t1, t2;				// variables for computing clocks 
	int *x, *y, s, p;
	double T1;
	int i, j, N;

	srand( time(NULL) );

	for(N=10;N<=10;N*=2)
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

		for(i=0;i<N;++i)
		{
			printf("x[%d]=%d\n",i,x[i]);
		}
		
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
		
		quicksort(y,0,N);
		for(i=0;i<N;++i)
		{
			printf("y[%d]=%d\n",i,y[i]);
		}
		
		free(x);
		free(y);
	} 

	return 0;
}

int quicksort(int *x, int left, int right)
{
	int i, j, k;
	int pivot, t;
	
	if(left < right-1)
	{
		pivot = x[left];
    	i = left;
    	j = right;
    	// 56970341
    	while(1)
		{
      		while(i < right && pivot >= x[i]) i++; 
      		while(j > left && pivot <= x[j]) j--; 
      		//printf("%d %d %d\n", i,j,pivot);
      		if(i>=j) break;
      		t = x[i];
      		x[i] = x[j];
      		x[j] = t;
			//for(k=left;k<right;++k)
			{
				//printf("x[%d]=%d\n",k,x[k]);
			}
			//system("pause");
        }
        t = x[left];
        x[left] = x[j];
        x[j] = t;
        //printf("%d\n",j);
		for(k=left;k<right;++k)
		{
			//printf("x[%d]=%d\n",k,x[k]);
		}
		//system("pause");
        
		quicksort(x, left, j);
		quicksort(x, j+1, right);
    }
    else 
    {
    	return 1;
	}
}
