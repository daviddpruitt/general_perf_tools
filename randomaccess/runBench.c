#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include "randomaccess.h"
#include "PerfCounts.h"

uint64_t dvmGetRTCTimeNsec()
{
  struct timeval now;
  gettimeofday(&now,NULL);
  return ((	uint64_t)now.tv_sec*1000000000LL + now.tv_usec*1000000LL);
}

int main(void)
{
	HPCC_Params *params;
	double *GUPs;
	double *cpuTime;
	double *realTime;
	u64Int *tableSize;
	int *failure;
	int doIO;
	uint64_t *results;//
	FILE *outFile = NULL;
	FILE *outFileCSV = NULL;
	
	outFile = fopen( "resultsRandomAccess.txt", "a" );
    if (! outFile) {
      outFile = stderr;
      fprintf( outFile, "Cannot open output file.\n" );
      return 1;
    }
    
    outFileCSV = fopen( "resultsRandomAccess.csv", "a" );
    if (! outFileCSV) {
      outFileCSV = stderr;
      fprintf( outFileCSV, "Cannot open output file.\n" );
      return 1;
    }
    
	params = (HPCC_Params *)malloc(sizeof(HPCC_Params));
	strcpy(params->outFname, "test.txt"); 
	params->HPLMaxProcMem = 1 * 1024 * 1024 * 1024;
	
	failure = malloc(sizeof(int));
	GUPs = malloc(sizeof(double));
	tableSize = malloc(sizeof(u64Int));
	results = malloc(sizeof(uint64_t) * 10);
	realTime = malloc(sizeof(double));
	cpuTime = malloc(sizeof(double));
	
	doIO = 1;
	results[0] = results[1] = results[2] = results[3] = results[4] = 0;
	
	
/*	printf("Mem size %u\n",params->HPLMaxProcMem);*/
/*	HPCC_RandomAccess(params, doIO, GUPs, failure); */
	
	/* Get start time so we can identify seperate runs */
	struct timeval startTime;
    gettimeofday(&startTime,NULL);
    time_t mytime;
    mytime = time(NULL);
    char *timeStart = ctime(&mytime);        
	fprintf(outFile,"@headerRandomAccess{\"starttime\":%s}",timeStart);
	
	double mfact = pow(2, 0.25);
	float kb;
	
	for(kb = 4; kb <= (64*1024); kb *= mfact) {
		params->HPLMaxProcMem = kb * 1024;
		printf("################### currently testing %f kb ###################\n",kb);
		HPCC_RandomAccess(params, doIO, GUPs, cpuTime, realTime, failure, tableSize, results);
		u64Int tableSizet = *tableSize;
		fprintf(outFile,"@randomAccess{\"size-kb\":%f,\"GUPs\":%f,\"table-entries\":"FSTR64",\"tableSize-bytes\":"FSTR64"}",kb,*GUPs,*tableSize,*tableSize * sizeof(u64Int));
		fprintf(outFileCSV,"%f,%f,%"PRIu64","FSTR64","FSTR64",%f,%f,"FSTR64","FSTR64","FSTR64","FSTR64","FSTR64","FSTR64","FSTR64","FSTR64","FSTR64"\n",
			kb, 
			*GUPs, 
			(dvmGetRTCTimeNsec() / 1000),
			tableSizet, 
			(*tableSize * sizeof(u64Int)),
			*cpuTime,
			*realTime,
            results[0],
            results[1],
            results[2],
            results[3],
            results[4],
            results[5],
            results[6],
            results[7],
            results[8]);
/*		printf("Table Size %u",*tableSize);*/
		printf("******************* testing %f kb complete  *******************\n",kb);
		fflush(outFileCSV);
	}
	
	fprintf(outFile,"\n");
	fclose(outFile);
	fclose(outFileCSV);
	
	return(0);
		
}

