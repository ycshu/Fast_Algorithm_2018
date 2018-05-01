#include <stdio.h>

int main()
{
	bit_reverse(8);
	return;
}

int bit_reverse(int N)
{
    int m,p,q,k;
    m = N/2;
    q = m;
    for(p=1;p<N-1;++p)
    {
        printf("%d <-> %d\n", p,q);
        if(p < q)
        {
            //swap p and q
        }
        k = m;
        while(q >= k & k > 0)
        {
            q = q-k;
            k = k/2;
        }
        q = q+k;
    }
    return 0;
}
