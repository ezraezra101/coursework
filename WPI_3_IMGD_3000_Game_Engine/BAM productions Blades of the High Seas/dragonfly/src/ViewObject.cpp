#include "ViewObject.h"
#include "WorldManager.h"
#include "LogManager.h"
#include "utility.h"
#include "GraphicsManager.h"
#include "EventView.h"

//Construct ViewObject.
//Object settings: SPECTRAL, max alt.
//ViewObject defaults: border, top_center, default color

df::ViewObject::ViewObject(){

	//Initialize object attributes
	setSolidness(SPECTRAL);
	setAltitude(MAX_ALTITUDE);
	setType("ViewObject");

	//Initialize ViewObject attributes
	setValue(0);
	setBorder(true);
	setLocation(TOP_CENTER);
	setColor(COLOR_DEFAULT);
}

//Draw view string and value
void df::ViewObject::draw(){
	printf("Drawing view object\n");
	//Display view_string + value + outer edges of border.
	LogManager &log_manager = LogManager::getInstance();
	
	std::string temp_str;
	if (border){
		temp_str = "| " + getViewString() + " " + std::to_string(value) + " |";
	}
	else{
		temp_str = getViewString() + " " + std::to_string(value);
	}
	
	Position pos = viewToWorld(getPosition());//grab the position
	GraphicsManager &graphics_manager = GraphicsManager::getInstance();
	graphics_manager.drawString(pos, temp_str.c_str(), CENTER_JUSTIFIED, getColor());//draw the display object

	if (border){//if there is a border, draw that too.
		pos.setY(pos.getY() + 1);//move down one space (just below the viewobject)
		std::string bord_string;//make a border string
		for (int i = 2; i < temp_str.length(); i++){
			bord_string.append("-");
		}
		graphics_manager.drawString(pos, bord_string, CENTER_JUSTIFIED, getColor());//draw the bottom border
		pos.setY(pos.getY() - 2);//move up two spaces (just above the viewobject
		graphics_manager.drawString(pos, bord_string, CENTER_JUSTIFIED, getColor());//draw the top border
	}
}

//Handle 'view' event if tag matches view_string (others ignored).
//Return 0 if ignored, else 1 if handled.
int df::ViewObject::eventHandler(const Event *p_e){
	if ((p_e->getType()) == VIEW_EVENT){//if it gets a step event, just return true. This may matter later.
		const EventView *p_ve = static_cast <const EventView *> (p_e);

		//See if this event is meant for this object
		if (p_ve->getTag() == getViewString()){
			if (p_ve->getDelta()){
				setValue(getValue() + p_ve->getValue());	//change in value
			}
			else{
				setValue(p_ve->getValue());
			}
			return 1;
		}

		
	}

	//If here, event was not handled.
	return 0;
}

void df::ViewObject::setLocation(ViewObjectLocation new_location){
	//set new position based on location
	WorldManager &world_manager = WorldManager::getInstance();
	Position new_pos = df::Position();
	LogManager &log_manager = LogManager::getInstance();
	int delta = 0;
	switch (new_location){//Assign the proper position based on given location
	case TOP_LEFT:
		new_pos.setXY(world_manager.getView().getHorizontal() * 1 / 6, 1);
		
		if (!getBorder()){
			delta = -1;
		}
		break;
	case TOP_CENTER:
		
		
		new_pos.setXY(world_manager.getView().getHorizontal() * 3 / 6, 1);
		
		if (!getBorder()){
			delta = -1;
		}
		break;
	case TOP_RIGHT:
		new_pos.setXY(world_manager.getView().getHorizontal() * 5 / 6, 1);
		
		if (!getBorder()){
			delta = -1;
		}
		break;
	case CENTER_CENTER:
		new_pos.setXY(world_manager.getView().getHorizontal() * 3 / 6, world_manager.getView().getVertical() * 3 / 6);
		
		if (!getBorder()){
			delta = -1;
		}
		break;

	case BOTTOM_LEFT:
		new_pos.setXY(world_manager.getView().getHorizontal() * 1 / 6, world_manager.getView().getVertical() * 5 / 6);
		
		if (!getBorder()){
			delta = -1;
		}
		break;
	case BOTTOM_CENTER:
		new_pos.setXY(world_manager.getView().getHorizontal() * 3 / 6, world_manager.getView().getVertical() * 5 / 6);
		
		if (!getBorder()){
			delta = -1;
		}
		break;
	case BOTTOM_RIGHT:
		new_pos.setXY(world_manager.getView().getHorizontal() * 5 / 6, world_manager.getView().getVertical() * 5 / 6);
		
		if (!getBorder()){
			delta = -1;
		}
		break;
	}
	//Shift, as needed, based on border.
	new_pos.setY(new_pos.getY() + delta);
	//Set position of object to new position.
	setPosition(new_pos);

	location = new_location;

	
}

//General location of ViewObject on screen.
df::ViewObjectLocation df::ViewObject::getLocation() const{
	return location;
}

//Set view value.
void df::ViewObject::setValue(int new_value){
	value = new_value;
}

//Get view value
int df::ViewObject::getValue() const{
	return value;
}
//Set View border (true = display border).
void df::ViewObject::setBorder(bool new_border){
	if (border != new_border){
		border = new_border;
		//reset location to account for border setting.
		setLocation(getLocation());
	}
}
//Get View border (true = display border).
bool df::ViewObject::getBorder() const{
	return border;
}

//Set view color.
void df::ViewObject::setColor(Color new_color){
	color = new_color;
}
//Get view color.
df::Color df::ViewObject::getColor() const{
	return color;
}

//Set view display string.
void df::ViewObject::setViewString(std::string new_view_string){
	view_string = new_view_string;
}

//Get view display string.
std::string df::ViewObject::getViewString() const{
	return view_string;
}