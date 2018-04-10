#include <stdio.h>
#define DEBUG 0
int main()
{
	printf("Hello world!\n");
	
	#if (DEBUG)
	printf("DEBUG world\n");
	#endif
	
} 
