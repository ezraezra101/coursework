#ifndef __EVENT_KEYBOARD_H__
#define __EVENT_KEYBOARD_H__

#include "Event.h"

namespace df {

// Types of keyboard actions Dragonfly recognizes.
enum EventKeyboardAction {
	UNDEFINED_KEYBOARD_ACTION = -1,
	KEY_PRESSED,	// Was down
	KEY_RELEASED,	// Was released
	KEY_DOWN		// Is down
};

namespace Input {
	enum Key {
		UNDEFINED_KEY = -1,
		SPACE, RETURN, ESCAPE, TAB, LEFTARROW, RIGHTARROW, UPARROW, DOWNARROW,
		PAUSE, MINUS, PLUS, TILDE, PERIOD, COMMA, SLASH, LEFTCONTROL,
		RIGHTCONTROL, LEFTSHIFT, RIGHTSHIFT, F1, F2, F3, F4, F5, F6, F7, F8,
		F9, F10, F11, F12, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q,
		R, S, T, U, V, W, X, Y, Z, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7,
		NUM8, NUM9, NUM0,
	};
} // End of Input namespace

const std::string KEYBOARD_EVENT = "df::keyboard";

class EventKeyboard : public Event {
private:
	Input::Key key_val;						// Key value.
	EventKeyboardAction keyboard_action;	// Key action.
public:
	// Default constructor
	EventKeyboard();

	// Constructor with arguments
	EventKeyboard(Input::Key init_key_val, EventKeyboardAction init_action);

	// Set & get key in event.
	void setKey(Input::Key new_key);
	Input::Key getKey(void) const;

	// Set & get keyboard action
	void setAction(EventKeyboardAction action);
	EventKeyboardAction getAction(void) const;
	
};

}
#endif

