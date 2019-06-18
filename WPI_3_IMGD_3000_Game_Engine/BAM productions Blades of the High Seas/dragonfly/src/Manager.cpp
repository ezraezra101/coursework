#include "Manager.h"
#include "WorldManager.h"
#include "ObjectList.h"
#include "ObjectListIterator.h"

//Set type identifier of Manager.
void df::Manager::setType(std::string manager_type) {
	type = manager_type;
}

df::Manager::Manager() {
	type = df::UNDEFINED_MANAGER;
	is_started = false;
}

df::Manager::~Manager() {
}


std::string df::Manager::getType() const {
	return type;
}

// Return 0 if OK, else a negative number
int df::Manager::startUp() {
	is_started = true;
	return 0;
}

void df::Manager::shutDown() {
	is_started = false;
}

// Return true when startUp() was executed successfully, else false.
bool df::Manager::isStarted() {
	return is_started;
}

// Send event to all Objects & return count of number of events sent.
int df::Manager::onEvent(const df::Event *p_event) const {
	int count = 0;

	df::ObjectList all_objects = df::WorldManager::getInstance().getAllObjects();
	df::ObjectListIterator object_iterator(&all_objects);

	while(!object_iterator.isDone()) {
		object_iterator.currentObject()->eventHandler(p_event);
		object_iterator.next();
		count++;
	}
	return count;
}