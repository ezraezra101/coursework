
#include "EventCollision.h"

int main() {

	int failed_tests = 0;

	df::Position pos(1,1);
	df::Object *o1 = new df::Object(), *o2 = new df::Object();
	df::EventCollision ce = df::EventCollision(o1, o2, pos),
		default_ce = df::EventCollision();

	if( default_ce.getObject1() != NULL ||
		default_ce.getObject2() != NULL ||
		default_ce.getPosition().getX() != 0) {
		printf("EventCollision defaults are improperly set.\n");
		failed_tests++;
	}

	if( ce.getObject1() != o1 ||
		ce.getObject2() != o2 ||
		ce.getPosition().getX() != 1) {
		printf("EventCollision constructor doesn't work.\n");
		failed_tests++;
	}

	df::Position pos2(3,3);
	ce.setObject1(o2);
	ce.setObject2(NULL);
	ce.setPosition(pos2);
	if( ce.getObject1() != o2 ||
		ce.getObject2() != NULL ||
		ce.getPosition().getX() != 3) {
		printf("EventCollision getters/setters doesn't work.\n");
		failed_tests++;
	}

	return failed_tests;
}