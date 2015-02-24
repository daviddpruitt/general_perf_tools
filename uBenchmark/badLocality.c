#include "stdio.h"
#include "utility.h"

int* array;

int*
makeList(int nPages)
{
	int *a = array;
	int i;

	for(i=0; i<1024*(nPages-1); i++)
		a[i] = i+1024;

	for(i=0; i<1023; i++)
		a[(nPages-1)*1024+i] = i+1;

	a[nPages*1024-1] = -1;

	return a;
}

long long
walk(int  *a, long long unsigned iter)
{
	int r=0;
	int i=0;
	long long ops = 0;
	while (iter--) {
		while(a[i] != -1) {
			//			System.out.println("a[i]="+a[i]);
			r += a[i];
			i = a[i];
			ops++;	
		}
	}

	return r;
}
        
        
        
long long 
longOperation() {
	int nPages = 0;
	int *a;
	double time = 0;
	long long ops = 0;
	double otime = 0;
	long long unsigned nIters = 10;
	float kb;
	double cputime;
	array = (int*)malloc(sizeof(int)*32*1024*1024);
	
	//double mfact = 1.189207115;//Math.pow(2, 0.25);
	//int oNPages = 1;	// old size, in kb
	printf("Begin Loop\n");
	kb = SIZETOTEST;
/*	for(kb = 4; kb <= (64*1024); kb *= mfact) {*/
		nPages = (int)kb/4;
		a = makeList(nPages);
		ops = walk(a,2);
		nIters = 10; 
		printf("Testing %0.3f kb\n",kb);
		do {
			nIters *= 2;
			cputime = -HPL_timer_cputime();
			ops = walk(a, nIters);
			cputime += HPL_timer_cputime();
		} while (cputime < 15);

		//float actualMfact = nPages/oNPages;
		//int ratio = (int)(100.0*(float)time/(otime*mfact));
		printf("%0.3f kb %0.6f s %llu ops\n", kb, cputime, ops);
		otime = time;
/*	}*/
    return 0;
}

int
main(void)
{
	longOperation();
	return 0;
}
