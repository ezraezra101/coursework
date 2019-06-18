#include <stdio.h>

#include "EventKeyboard.h"

int main() {

	int failed_tests = 0;

	df::EventKeyboard ke = df::EventKeyboard(df::Input::A, df::KEY_DOWN),
		default_ke = df::EventKeyboard();

	if( default_ke.getKey() != df::Input::UNDEFINED_KEY ||
		default_ke.getAction() != df::UNDEFINED_KEYBOARD_ACTION) {
		printf("EventKeyboard defaults are improperly set.\n");
		failed_tests++;
	}
	if( ke.getKey() != df::Input::A ||
		ke.getAction() != df::KEY_DOWN ) {
		printf("EventKeyboard constructor doesn't work.\n");
		failed_tests++;
	}

	ke.setKey(df::Input::SPACE);
	ke.setAction(df::KEY_PRESSED);
	if( ke.getKey() !=  df::Input::SPACE ||
		ke.getAction()!=df::KEY_PRESSED) {
		printf("EventKeyboard getters/setters don't work.\n");
		failed_tests++;
	}

	return failed_tests;
}