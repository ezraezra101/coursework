#include <stdio.h>

#include "EventMouse.h"

int main() {

	int failed_tests = 0;

	df::Position pos(1,1);
	df::EventMouse me = df::EventMouse(df::LEFT, df::CLICKED,pos),
		default_me = df::EventMouse();

	if( default_me.getMouseButton() != df::UNDEFINED_MOUSE_BUTTON ||
		default_me.getMouseAction() != df::UNDEFINED_MOUSE_ACTION ||
		default_me.getMousePosition().getX() != 0) {
		printf("EventMouse defaults are improperly set.\n");
		failed_tests++;
	}
	if( me.getMouseButton() != df::LEFT ||
		me.getMouseAction() != df::CLICKED ||
		me.getMousePosition().getX() != 1) {
		printf("EventMouse constructor doesn't work.\n");
		failed_tests++;
	}

	df::Position pos2(3,3);
	me.setMouseButton(df::RIGHT);
	me.setMouseAction(df::PRESSED);
	me.setMousePosition(pos2);
	if( me.getMouseButton() != df::RIGHT   ||
		me.getMouseAction() != df::PRESSED ||
		me.getMousePosition().getX() != 3  ) {

		printf("EventMouse getters/setters don't work.\n");
		failed_tests++;
	}

	return failed_tests;
}