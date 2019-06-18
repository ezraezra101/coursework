#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h> // Allows us to seed random

#include "timing.h"

// Initializes an array of doubles of length n with values 0-100
double *setupVector(int n) {
	double *a = calloc(n, sizeof(double)); // Make 1 call to sbrk... assuming normal implementation of malloc.
	double drand_max = 100.0 / (double) RAND_MAX;

	srand((unsigned int) time(NULL)); // Note: this and rand are not secure, especially with multiple calls to srand.
	for(int i=0; i<n; i++) {
		a[i] = drand_max * (double) rand();
	}
	return a;
}

// Runs a simple math calculation on 4 vectors in the array defined as a.
// N is the length of a
void vectorTriadPerformance(double * a, int N) {

	//Setup
	int n = N/4;
	double *b,*c,*d;
	b =&a[n];
	c=&b[n];
	d=&c[n];

	// Test
	for(int i=0; i<n;i++) {
		a[i] = b[i]+c[i]*d[i];
	}
}


int main(int argc, char **argv) {

	printf("Trial \tLength \tWallClock \tCPU \tTotalCPU \tTotalWallClock \tTotalIterations\n");
	for(int i=3; i<=25; i++) {
		int length = floor(pow(2.1, i));
		double *a = setupVector(length*4);

		int count = 0;
		double totalWallClockTime = 0.0;
		double totalCpuTime = 0.0;


		int repeat = 1; // Use a silly little technique to limit the number of timing calls in the trial.
		double wcStart, cpuStart, wcEnd, cpuEnd;
		timing(&wcEnd, &cpuEnd); // Dumb initialization
		timing(&wcStart, &cpuStart);


		while(totalWallClockTime < 1.0) {

			for(int i=0; i<repeat; ++i) {
				vectorTriadPerformance(a, length*4);
				count += 1;
			}
			repeat *= 2;

			timing(&wcEnd, &cpuEnd);
			totalWallClockTime = wcEnd - wcStart;
			totalCpuTime = cpuEnd - cpuStart;
		}
		printf("%d \t%d \t%lf \t%lf \t%lf \t%lf \t%d\n",
			i, length,totalWallClockTime/count, totalCpuTime/count, totalWallClockTime,totalCpuTime,count
		);

		free(a);
	}
}