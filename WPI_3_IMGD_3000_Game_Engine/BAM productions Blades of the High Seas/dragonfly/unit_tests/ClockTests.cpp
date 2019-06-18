#include <stdio.h>

#include "Clock.h"

// Mac and linux only
#include <time.h>
void waitMicroseconds(int u_sec) {
	struct timespec sleep_time;
	sleep_time.tv_sec = u_sec / 1000000;
	sleep_time.tv_nsec= (u_sec % 1000000)*1000;

	nanosleep(&sleep_time, NULL);

	// Windows is just:
	// Sleep(u_sec/1000); // Only sleeps using milliseconds :-(
}

int main() {

	int failed_tests = 0;

	long int elapsed_time;

	df::Clock c = df::Clock();

	waitMicroseconds(10000);

	elapsed_time = c.delta();

	if(elapsed_time > 13000 || elapsed_time < 9000) {
		printf("Clock delta is inaccurate! (%ld instead of 10000)\n", elapsed_time);
		failed_tests++;
	}

	waitMicroseconds(5000);

	c.split();
	waitMicroseconds(5000);
	elapsed_time = c.split();

	if(elapsed_time > 13000 || elapsed_time < 9000) {
		printf("Clock split is inaccurate! (%ld instead of 10000)\n", elapsed_time);
		failed_tests++;
	}

	return failed_tests;
}