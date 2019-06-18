#include "LogManager.h"

int main() {
	int failed_tests = 0;

	df::LogManager &log_manager = df::LogManager::getInstance();

	// Not testing getType on base manager class

	if(log_manager.startUp()) {
		printf("LogManager didn't startup properly!\n");
		failed_tests++;
	}
	if(0 > log_manager.writeLog("testing 1 2 3... 1 2 3...")) {
		printf("Error writing log!\n");
		failed_tests++;
	}
	df::LogManager &log_manager_2 = df::LogManager::getInstance();
	if(0 > log_manager_2.writeLog("LogManager Testing new reference")) {
		printf("LogManager failed with two instances!");
	}
	
	log_manager.shutDown(); // No segfaulting, hopefully!

	if(log_manager.isStarted()) {
		printf("LogManager::isStarted not working properly after shutdown\n");
		failed_tests++;
	}

	if(log_manager.startUp()) {
		printf("LogManager failed to restart!");
		failed_tests++;
	}

	if(!log_manager.isStarted()) {
		printf("LogManager::isStarted not working properly after restart\n");
		failed_tests++;
	}

	if(0 > log_manager_2.writeLog("LogManager Testing restart")) {
		printf("LogManager write after restart failed!\n");
		failed_tests++;
	}

	// Let logManager self destruct

	return failed_tests;
}