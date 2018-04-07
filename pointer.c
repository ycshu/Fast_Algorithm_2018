#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
	int n;									// Open sizeof(int) memory for n
											// &n: the location of n in memory
											// for example, n = 1;
											// Put n at memory 132, 
											//     n is decided by Byte 132, 133, 134, 135
											//	  &n = 132;
	char *c;								// char, sizeof(char) = 1
	short *s;								// short, sizeof(short) = 2
	//long long L, L1, L2; 
								// *: pointer! a variable of memory
	//double *d = &L, *d1 = &L1, *d2 = &L2;
	

	n = 12345678;
	n = 0x04010302;							// c[0]=(02) c[1]=(03) c[2]=(01) c[3]=(04)
	c = &n;			
	s = &n;									// s[0] = (0203)  s[1] = (0104)
	printf("n = %d\n", n);
	printf("each location of n: %d %d %d %d\n", c[0], c[1], c[2], c[3]); // c[0]: c這個記憶體位置的值
													 // c[1]: c這個記憶體位置的下一個位置的值 
	printf("s: %d %d\n", s[0], s[1]); 				 // s[0]: c這個記憶體位置的值
													 // s[1]: c這個記憶體位置的下一個位置的值 
	printf("*c %d\n", *c);							 // *c = c[0]
	
	float *f = &n;								// sizeof(float) = 4,
	
	// (0111 1111 1000 0000 0000 0000 ...0)
	//     7 F    8    0 
	
	n = 0x7F800000;   // n ( 00 00 80 7F )
	printf("n = %d, f = %e\n", n, *f);

	// (0011 1111 1000 0000 0000 0000 ...0)
	//     3 F    8    0 
	n = 0x3F800000;   // n ( 00 00 80 3F )
	printf("n = %d, f = %e\n", n, *f);

	return;
}
