#ifndef __CLOCK_H__
#define __CLOCK_H__

// The clock, for timing (such as in the game loop)
namespace df{

class Clock {
private:
	long int previous_time; // Previous time delta() called (in microseconds)

public:
	// Sets previous_time to current time;
	Clock();

	// Return time elapsed since delta() was last called, -1 if error
	// Units in microseconds
	long int delta(void);

	// Return time elapsed since delata() was called, -1 if error.
	// Units are microseconds
	long int split(void) const;

};

}
#endif
