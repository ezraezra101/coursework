#include "GraphicsManager.h"
#include "WorldManager.h"
#include "ViewObject.h"
#include "utility.h"

// Construct ViewObject.
// Object settings: SPECTRAL, max_alt.
// ViewObject defaults: border, top_center, default color.
df::ViewObject::ViewObject() {

	// Initialize Object attributes.
	setSolidness(SPECTRAL);
	setAltitude(MAX_ALTITUDE);
	setType("ViewObject");

	// Initialize ViewObject attributes.
	setValue(0);
	setBorder(true);
	setLocation(TOP_CENTER);
	setColor(COLOR_DEFAULT);
}

// Draw view string and value.
void df::ViewObject::draw(void) {
	// Display view_string + value.
	std::string temp_str;
	if(border) {
		temp_str = " " + getViewString() + " " + toString(value) + " ";
	} else {
		temp_str = getViewString() + " " + toString(value);
	}

	// Draw centered at position.
	df::Position pos = worldToView(getPosition());
	df::GraphicsManager::getInstance().drawString(pos, temp_str, CENTER_JUSTIFIED, getColor());

	if(border) {
		// Draw box around display.
		// TODO
		df::GraphicsManager::getInstance().drawCh(df::Position(pos.getX()-temp_str.size()/2-1, pos.getY()),
			'#', getColor());
		df::GraphicsManager::getInstance().drawCh(df::Position(pos.getX()+temp_str.size()/2+1, pos.getY()),
			'#', getColor());
		for(int i=-1; i<temp_str.size() + 1; i++) {
			df::GraphicsManager::getInstance().drawCh(
				df::Position(pos.getX()-temp_str.size()/2 + i, pos.getY() + 1),
				'#', getColor());

			df::GraphicsManager::getInstance().drawCh(
				df::Position(pos.getX()-temp_str.size()/2 + i, pos.getY()-1),
				'#', getColor());
		}
		
	}
}

// Handle 'view' event if tag matches view_string (others ignored).
// return 0 if ignored, else 1 if handled.
int df::ViewObject::eventHandler(const df::Event *p_e) {

}

// General location of ViewObject on screen.
void df::ViewObject::setLocation(df::ViewObjectLocation new_loc) {
	df::Position p;
	df::WorldManager &wm = df::WorldManager::getInstance();
	int delta = 0;
	// set new posiiton based on location.
	switch(new_loc) {
	case TOP_LEFT:
		p.setXY(wm.getView().getHorizontal() /6, 1);
		if(getBorder() == false) {
			delta = -1;
		}
		break;
	case TOP_CENTER:
		p.setXY(wm.getView().getHorizontal() *3/6, 1);
		if(getBorder() == false) {
			delta = -1;
		}
		break;
	case TOP_RIGHT:
		p.setXY(wm.getView().getHorizontal() *5/6, 1);
		if(getBorder() == false) {
			delta = -1;
		}
		break;
	case CENTER_CENTER:
		p.setXY(wm.getView().getHorizontal()/2, 
			wm.getView().getVertical()/2);
		if(getBorder() == false) {
			delta = 0;
		}
		break;
	case BOTTOM_LEFT:
		p.setXY(wm.getView().getHorizontal()/6,
			wm.getView().getVertical() - 1);
		if(getBorder() == false) {
			delta = 1;
		}
		break;
	case BOTTOM_CENTER:
		p.setXY(wm.getView().getHorizontal() *3/6,
			wm.getView().getVertical() - 1);
		if(getBorder() == false) {
			delta = 1;
		}
		break;
	case BOTTOM_RIGHT:
		p.setXY(wm.getView().getHorizontal() *5/6, 
			wm.getView().getVertical() - 1);
		if(getBorder() == false) {
			delta = 1;
		}
		break;
	}

	// Shift as needed, based on border.
	p.setY(p.getY() + delta);

	// Set position of object to new position.
	setPosition(p);

	// Set new location.
	location = new_loc;
}
df::ViewObjectLocation df::ViewObject::getLocation() const {
	return location;
}

// Set/get view value.
void df::ViewObject::setValue(int new_value) {
	value = new_value;
}
int df::ViewObject::getValue() const {
	return value;
}

// Set/get view border (true = display border)
void df::ViewObject::setBorder(bool new_border) {
	if(new_border != border) {
		border = new_border;

		// Reset location to account for border setting.
		setLocation(getLocation());
	}
}
bool df::ViewObject::getBorder() const {
	return border;
}

// Get/set view color
void df::ViewObject::setColor(df::Color c) {
	color = c;
}
df::Color df::ViewObject::getColor() const {
	return color;
}

// Set/Get view display string.
void df::ViewObject::setViewString(std::string new_str) {
	view_string = new_str;
}
std::string df::ViewObject::getViewString() const {
	return view_string;
}