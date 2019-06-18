#ifndef EVENT_LEVEL_UP_H
#define EVENT_LEVEL_UP_H

#include "Event.h"

const std::string LEVEL_UP_EVENT = "EventLevelUp";

class EventLevelUp : public df::Event {
public:
	EventLevelUp();
};
#endif