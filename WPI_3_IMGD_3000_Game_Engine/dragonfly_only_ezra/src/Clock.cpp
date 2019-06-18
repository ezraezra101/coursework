
#include "Clock.h"

// Methods of getting current time vary from operating system to operating system.
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WINDOWS) || defined(_WIN64) || defined(__TOS_WIN__)
	#include <Windows.h>

	// Returns current time in microseconds
	static inline long int currentTime() {
		SYSTEMTIME st;
		GetSystemTime(&st);
		return st.wMinute * 60 * 1000000 + st.wSecond * 1000000 + st.wMilliseconds * 1000;
	}

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__) || defined(__MACH__) || defined(Macintosh)
	#include <sys/time.h>

	// Returns current time in microseconds
	static inline long int currentTime() {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec*1000000 + tv.tv_usec;
	}

#else // Assuming Linux
	// Compile with -lrt!
	#include <time.h>

	// Returns current time in microseconds
	static inline long int currentTime() {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		return ts.tv_sec*1000000 + ts.tv_nsec/1000;
	}
#endif

df::Clock::Clock() {
	previous_time = currentTime();
}

long int df::Clock::delta(void) {
	long int elapsed_time = split();
	previous_time = currentTime();

	return elapsed_time;
}

long int df::Clock::split(void) const {
	return currentTime() - previous_time;
}
