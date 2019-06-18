#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Event.h"

#include <string>

namespace df {

const std::string UNDEFINED_MANAGER = "df::undefined";

class Manager {
private:
	std::string type;		//Manager type identifier
	bool is_started;		// True when started successfully

protected:
	//Set type identifier of Manager.
	void setType(std::string type);

public:
	Manager();
	virtual ~Manager();

	// Get type identifier of Manager.
	std:: string getType() const;

	// Startup Manager
	// Return 0 if OK, else a negative number.
	virtual int startUp();

	// Shutdown manager
	virtual void shutDown();

	// Return true when startUp() was executed successfully, else false.
	bool isStarted();

	// Send event to all Objects & return count of number of events sent.
	int onEvent(const df::Event *p_event) const;

};

}

#endif