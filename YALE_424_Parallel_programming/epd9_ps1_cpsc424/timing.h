// This file isn't transferred over to GRACE, but works locally.
#ifndef TIMING_H
#define TIMING_H

#include <sys/time.h>
#include <time.h>

// My version of the timing function.
// This code is copied from https://stackoverflow.com/questions/17432502/how-can-i-measure-cpu-time-and-wall-clock-time-on-both-linux-windows
void timing(double* wallClockTime, double* cpuTime) {
    *cpuTime = (double)clock() / CLOCKS_PER_SEC;

    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        *wallClockTime = 0;
    } else {
    	*wallClockTime = (double)time.tv_sec + (double)time.tv_usec * .000001;
    }
}

#endif