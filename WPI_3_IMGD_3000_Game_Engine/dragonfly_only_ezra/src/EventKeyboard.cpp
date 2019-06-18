#include "EventKeyboard.h"

df::EventKeyboard::EventKeyboard() {
	setType(df::KEYBOARD_EVENT);
	key_val = Input::UNDEFINED_KEY;
	keyboard_action = UNDEFINED_KEYBOARD_ACTION;
}

df::EventKeyboard::EventKeyboard(df::Input::Key init_key_val, df::EventKeyboardAction init_action) {
	setType(df::KEYBOARD_EVENT);
	key_val = init_key_val;
	keyboard_action = init_action;
}

void df::EventKeyboard::setKey(df::Input::Key new_key) {
	key_val = new_key;
}

df::Input::Key df::EventKeyboard::getKey() const {
	return key_val;
}

void df::EventKeyboard::setAction(df::EventKeyboardAction action) {
	keyboard_action = action;
}

df::EventKeyboardAction df::EventKeyboard::getAction() const {
	return keyboard_action;
}
