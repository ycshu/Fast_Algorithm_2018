#include <stdio.h>

int main()
{
	bit_reverse(16);
	return;
}

int bit_reverse(int N)
{
    int m,p,q,k;
    m = N/2;                        // Bit-Reverse 每次要進位的數字 
    q = m;							// p = 1, q = m (第一個要交換的) 
    for(p=1;p<N-1;++p)
    {
        printf("%d <-> %d\n", p,q);
        if(p < q)
        {
            //swap p and q
        }
        k = m;						// k, 用來檢查第 log_2 k + 1 位是不是1 
        while(q >= k & k > 0)		// q >=k 第 (log_2 k + 1)位是1,  
        {
            q = q-k;				// 1->0
            k = k/2;				// 檢查下一位 
        }
        q = q+k;
    }
    return 0;
}
