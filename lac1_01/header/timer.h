#ifndef _TIMER_
#define _TIMER_

#include <unordered_map>
#include <string>
#include <sys/timeb.h>
#include <ctime>
#include <sys/time.h>
#include <cstdio>

using namespace std;

#define __START_TIMER__   struct timeval start; gettimeofday(&start, NULL);
#define __FINISH_TIMER__  struct timeval end; gettimeofday(&end, NULL); PROOF[__FUNCTION__]+=(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec;

extern unordered_map<string, unsigned long long> PROOF;//defined in main.cpp

void printTime(void)
{
	for(pair<const string, unsigned long long> &p : PROOF){
		printf("time: %s %f\n", p.first.c_str(), p.second/(double)1000000);
	}
	printf("\n");
}

#endif
