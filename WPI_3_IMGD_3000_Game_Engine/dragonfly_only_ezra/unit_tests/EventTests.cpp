
#include "Event.h"

int main() {

	int failed_tests = 0;

	df::Event e;

	if(!(e.getType() == df::UNDEFINED_EVENT)) {
		printf("Event's default type isn't set properly. Type is: %s\n", df::UNDEFINED_EVENT.c_str());
		failed_tests++;
	}
	std::string event_name = "ArbitraryEvent";
	e.setType(event_name);
	if(!(e.getType() == event_name)) {
		printf("Event::setType() isn't working. Type is: %s\n", e.getType().c_str());
		failed_tests++;
	}

	return failed_tests;
}