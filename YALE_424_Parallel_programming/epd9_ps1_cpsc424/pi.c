#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "timing.h"

long double PI = 3.14159265358979323846264338;

// Approximate PI. More iterations is more accurate.
long double approximatePi(int n) {
	// Note: `long int n` instead of int n makes it take longer... (at optimization level 0 at least)
	// So I am using int. Please don't overflow n!
	long double delta_x = 1.0/(long double) n;
	long double x_i = -delta_x / 2.0;

	long double pi = 0.0;

	for(int i=0; i < n; i++) {
		// 3 adds, 2 multiplies, and whatever the for loop takes to evaluate...
		x_i += delta_x;

		long double f_x = 1.0 / (1.0 + x_i*x_i);

		pi += f_x * delta_x;
	}
	return 4*pi;
}


int main(int argc, char **argv) {
	int iterations = pow(2, 29); // Roughly half a billion iterations.

	double wcStart, cpuStart, wcEnd, cpuEnd;
	timing(&wcStart, &cpuStart);

	long double approx = approximatePi(iterations);
	
	timing(&wcEnd, &cpuEnd);

	if(fabsl(PI - approx) > 0.0000001) {
		printf("Error: Approximate value isn't PI: %Lf\n", approx);
	}
	printf("CPU\t%f\tWallClock\t%f\n", cpuEnd-cpuStart, wcEnd-wcStart);
}