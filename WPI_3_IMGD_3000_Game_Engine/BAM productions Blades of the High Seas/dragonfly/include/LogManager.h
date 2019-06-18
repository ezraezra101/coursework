#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

// System includes
#include <stdio.h>

// Engine includes
#include "Manager.h"

namespace df {

const std::string LOGFILE_NAME = "dragonfly.log";

class LogManager : public Manager {
private:
	bool do_flush; // True if fflush after each write
	FILE *p_f;	   // Pointer to logfile structure

	LogManager();
	LogManager(LogManager const &);
	void operator=(LogManager const&);


public:
	// If logfile is open, close it.
	~LogManager();

	static LogManager &getInstance();

	// Start up the LogManager (default logfile "dragonfly.log").
	int startUp();

	// Shut down the LogManager (close logfile).
	void shutDown();

	// Set flush of logfile after each write.
	void setFlush(bool do_flush=true);

	// Write to logfile. Supports printf() formatting.
	// Return number of bytes written, -1 if error.
	int writeLog(const char *fmt, ...);
};

}

// Handy macro that makes it slightly faster to log things.
#define LOG(...) df::LogManager::getInstance().writeLog(__VA_ARGS__)

#endif