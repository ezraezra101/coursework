#include <stdio.h>

#include "Object.h"
#include "WorldManager.h" // for MAX_ALTITUDE

int main() {

	int failed_tests = 0;

	df::Object o, o2;

	if(o.getId() == o2.getId()) {
		printf("Object ids are trivially repetitive.\n");
		failed_tests++;
	}
	o.setId(-5);
	if(o.getId() != -5) {
		printf("Object setId doesn't work.\n");
		failed_tests++;
	}

	o.setType("SillyObject");
	if(o.getType() != "SillyObject") {
		printf("Object type doesn't work.\n");
		failed_tests++;
	}

	o.setPosition(df::Position(1,2));
	if(o.getPosition().getX() != 1 || o.getPosition().getY() != 2) {
		printf("Object setPosition doesn't work.\n");
		failed_tests++;
	}

	df::Event e;
	if(o.eventHandler(&e)) {
		printf("Object claims to handle arbitrary events.\n");
		failed_tests++;
	}

	if(o.getAltitude() != df::MAX_ALTITUDE/2) {
		printf("Object Altitude not properly initialized\n");
		failed_tests++;
	}

	if(o.setAltitude(0) || o.getAltitude() != 0) {
		printf("Object Altitude unable to be properly set!\n");
		failed_tests++;
	}

	if(o.getSolidness() != df::SOFT) {
		printf("Object default solidness isn't SOFT!\n");
		failed_tests++;
	}

	df::Sprite *sprite = new df::Sprite(0);
	o.setSprite(sprite);
	o.setCentered(false);
	o.setSpriteIndex(0);
	o.setSpriteSlowdown(1);
	o.setSpriteSlowdownCount(2);

	if(o.isCentered() || o.getSpriteIndex() != 0 ||
		o.getSpriteSlowdown() != 1 || o.getSpriteSlowdownCount() != 2 ||
		o.getSprite() != sprite) {
		printf("Object's sprite related getters/setters don't work!\n");
		failed_tests++;
	}


	if(o.getBox().getHorizontal() != sprite->getWidth()-1 || o.getBox().getVertical() != sprite->getHeight()-1  ) {
		printf("Object's default bounding box is wrong!\n");
		failed_tests++;
	}
	o.setBox(df::Box(df::Position(1,1), 1,1));

	if(o.getBox().getHorizontal() != 1) {
		printf("Object::setBox doesn't work!\n");
		failed_tests++;
	}

	return failed_tests;
}