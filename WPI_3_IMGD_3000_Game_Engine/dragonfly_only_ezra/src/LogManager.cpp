#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "LogManager.h"

// Not thread safe!
static char *getTimeString() {
	static char time_str[30];

	// System calls to get time.
	time_t now;
	time(&now);

	struct tm *p_time = localtime(&now);

	sprintf(time_str, "%02d:%02d:%02d",
		p_time->tm_hour,
		p_time->tm_min,
		p_time->tm_sec);


	return time_str;
}

df::LogManager::LogManager() {
	p_f = NULL;
	do_flush = false;

	setType("LogManager");
}

df::LogManager::LogManager(df::LogManager const &other) {
	p_f = other.p_f;
	do_flush = other.do_flush;

	setType("LogManager");
}

void df::LogManager::operator=(LogManager const &other) {
	p_f = other.p_f;
	do_flush = other.do_flush;
}

// If logfile is open, close it.
df::LogManager::~LogManager() {
	shutDown();
}

df::LogManager &df::LogManager::getInstance() {
	static df::LogManager *log_manager = new df::LogManager();
	return *log_manager;
}

// Start up the LogManager (default logfile "dragonfly.log").
int df::LogManager::startUp() {
	setType("LogManager");

	p_f = fopen(LOGFILE_NAME.c_str(), "a"); // Doesn't overwrite past logfiles

	if(p_f != NULL) {
		return df::Manager::startUp();
	} else {
		return -1;
	}
}

// Shut down the LogManager (close logfile).
void df::LogManager::shutDown() {
	if(p_f != NULL) {
		fflush(p_f);
		fclose(p_f);
		p_f = NULL;
	}

	df::Manager::shutDown();
}

// Set flush of logfile after each write.
void df::LogManager::setFlush(bool new_do_flush) {
	do_flush = new_do_flush;
}

// Write to logfile. Supports printf() formatting.
// Return number of bytes written, -1 if error.
int df::LogManager::writeLog(const char *fmt, ...) {
	if(p_f == NULL)
		return -1;

	int charCount = 0, errCheck;
	charCount = fprintf(p_f, "%s: ", getTimeString());
	if(charCount < 0) {
		return -1;
	}


	va_list args;
	va_start (args , fmt);
	charCount +=  (errCheck = vfprintf(p_f, fmt, args));
	if(errCheck < 0) {
		va_end(args);
		return -1;
	}
	va_end(args);

	charCount += (errCheck = fprintf(p_f, "\n"));
	if(errCheck < 0) {
		return -1;
	}

	if(do_flush) {
		fflush(p_f);
		// Doesn't force kernel to flush to disk, but I think that should be fine.
	}

	return charCount;
}
