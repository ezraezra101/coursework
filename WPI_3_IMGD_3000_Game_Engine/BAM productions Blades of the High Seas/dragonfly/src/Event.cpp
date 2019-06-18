#include "Event.h"


df::Event::Event() {
	event_type = df::UNDEFINED_EVENT;
}

df::Event::~Event() {

}

void df::Event::setType(std::string new_type) {
	event_type = new_type;
}

std::string df::Event::getType() const {
	return event_type;
}
