#include "ViewObject.h"
#include "WorldManager.h"

int main() {

	int failed_tests = 0;

	df::ViewObject *vo = new df::ViewObject();

	// Object settings: SPECTRAL, max_alt.
	// ViewObject defaults: border, top_center, default color.
	if( vo->getSolidness() != df::SPECTRAL ||
		vo->getAltitude()  != df::MAX_ALTITUDE ||
		vo->getLocation()  != df::TOP_CENTER ||
		vo->getColor()     != df::COLOR_DEFAULT ||
		vo->getBorder()    != true ||
		vo->getValue()     != 0) {
		printf("Default view object settings are wrong.\n");
		failed_tests++;
	}

	vo->setColor(df::BLUE);
	vo->setBorder(false);
	vo->setValue(3);
	vo->setViewString("hi");

	df::Position p = vo->getPosition();
	vo->setLocation(df::CENTER_CENTER);


	if(vo->getColor() != df::BLUE ||
		vo->getBorder()!=false || 
		vo->getValue() != 3 ||
		vo->getViewString() != "hi" ||
		vo->getLocation() != df::CENTER_CENTER ||
		vo->getPosition().getY() != p.getY()) {
		printf("View objects can't set colors.\n");
		failed_tests++;
	}

	return failed_tests;
}