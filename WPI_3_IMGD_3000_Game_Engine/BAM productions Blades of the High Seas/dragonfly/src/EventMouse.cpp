#include "EventMouse.h"

df::EventMouse::EventMouse() {
	setType(df::MOUSE_EVENT);
	btn = df::UNDEFINED_MOUSE_BUTTON;
	action = UNDEFINED_MOUSE_ACTION;
	mouse_xy = Position();
}

df::EventMouse::EventMouse(EventMouseButton init_btn, EventMouseAction init_action, Position pos) {
	setType(df::MOUSE_EVENT);
	btn = init_btn;
	action = init_action;
	mouse_xy = pos;
}

void df::EventMouse::setMouseButton(df::EventMouseButton new_button) {
	btn = new_button;
}

df::EventMouseButton df::EventMouse::getMouseButton() const {
	return btn;
}

void df::EventMouse::setMouseAction(df::EventMouseAction new_action) {
	action = new_action;
}

df::EventMouseAction df::EventMouse::getMouseAction() const {
	return action;
}

void df::EventMouse::setMousePosition(df::Position pos) {
	mouse_xy.setXY(pos.getX(), pos.getY());
}

df::Position df::EventMouse::getMousePosition() const {
	return mouse_xy;
}