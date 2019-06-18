#ifndef __EVENT_MOUSE_H__
#define __EVENT_MOUSE_H__

#include "Event.h"
#include "Position.h"

namespace df {

// Set of mouse buttons recognized by Dragonfly.
enum EventMouseButton {
	UNDEFINED_MOUSE_BUTTON = -1,
	LEFT,
	RIGHT,
	MIDDLE,
};

enum EventMouseAction {
	UNDEFINED_MOUSE_ACTION = -1,
	CLICKED,
	PRESSED,
	MOVED,
};

const std::string MOUSE_EVENT = "df::mouse";

class EventMouse : public Event {
private:
	EventMouseButton btn;	// Mouse button
	EventMouseAction action;// Mouse action
	Position mouse_xy;		// Mouse (x,y) coordinates
public:
	// Default constructor
	EventMouse();

	// Constructor with arguments
	EventMouse(EventMouseButton init_btn, EventMouseAction init_action, Position pos);


	// Set & get key in event.
	void setMouseButton(EventMouseButton new_key);
	EventMouseButton getMouseButton(void) const;

	// Set & get mouse action
	void setMouseAction(EventMouseAction action);
	EventMouseAction getMouseAction(void) const;
	
	// Set & get mouse coordinates
	void setMousePosition(Position new_mouse_xy);
	Position getMousePosition() const;
};

}
#endif

