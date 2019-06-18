#include <math.h> // For floor and fmod

#include "GraphicsManager.h"
#include "LogManager.h"
#include "Object.h"
#include "WorldManager.h"

df::Object::Object() {
	static int id_count = 0;
	id_count++;
	id = id_count;
	type = df::UNDEFINED_OBJECT;
	altitude = df::MAX_ALTITUDE/2;
	solidness = df::SOFT;

	x_velocity = 0;
	y_velocity = 0;
	x_velocity_countdown = 0;
	y_velocity_countdown = 0;

	pos = Position();

	p_sprite = NULL;
	sprite_index = 0;
	sprite_slowdown = 1;
	sprite_slowdown_count = 0;
	sprite_center = false;

	box = Box();

	WorldManager::getInstance().insertObject(this);
}

df::Object::~Object() {
	WorldManager::getInstance().removeObject(this);
}

void df::Object::setId(int new_id) {
	id = new_id;
}

int df::Object::getId() const {
	return id;
}

void df::Object::setType(std::string new_type) {
	type = new_type;
}

std::string df::Object::getType() const {
	return type;
}

void df::Object::setPosition(Position new_pos) {
	pos.setXY(new_pos.getX(), new_pos.getY());
}

df::Position df::Object::getPosition() const {
	return pos;
}

// Set altitude of object with checks for range [0, MAX_ALTITUDE]
// Return 0 if ok, -1 if not.
int df::Object::setAltitude(int new_altitude) {
	if(new_altitude > df::MAX_ALTITUDE || new_altitude < 0) {
		return -1;
	} else {
		altitude = new_altitude;
		return 0;
	}
}
// Return altitude of object
int df::Object::getAltitude() const {
	return altitude;
}

// Handle events (returns 1 if event is handled, 0 if not)
int df::Object::eventHandler(const Event *p_e) {
	return 0;
}

// Get and set velocity (x,y).
void df::Object::setXVelocity(float new_x_velocity) {
	x_velocity = new_x_velocity;
}
float df::Object::getXVelocity() const {
	return x_velocity;
}
void df::Object::setYVelocity(float new_y_velocity) {
	y_velocity = new_y_velocity;
}
float df::Object::getYVelocity() const {
	return y_velocity;
}

// Perform 1 step of velocity in horizontal direction.
// Return horizontal distance moved this step.
int df::Object::getXVelocityStep() {
	if(x_velocity == 0) {
		return 0;
	}
	// See if moving this step.
	x_velocity_countdown -= fabs(x_velocity);
	if(x_velocity_countdown > 0) {
		return 0; // Not time to move.
	}

	// Moving this step, so figure out how far.
	int spaces = floor(1 - x_velocity_countdown);
	x_velocity_countdown = 1 + fmod(x_velocity_countdown, 1);

	// Return number of spaces to move.
	if(x_velocity > 0) {
		return spaces;
	} else {
		return -1 * spaces;
	}
}

// Perform 1 step of velocity in vertical direction.
// Return vertical distance moved this step.
int df::Object::getYVelocityStep() {
	if(y_velocity == 0) {
		return 0;
	}

	// See if moving this step.
	y_velocity_countdown -= fabs(y_velocity);
	if(y_velocity_countdown > 0) {
		return 0; // Not time to move.
	}

	// Moving this step, so figure out how far.
	int spaces = floor(1 - y_velocity_countdown);
	y_velocity_countdown = 1 + fmod(y_velocity_countdown, 1);

	// Return number of spaces to move.
	if(y_velocity > 0) {
		return spaces;
	} else {
		return -1 * spaces;
	}
}

// True if HARD or SOFT, else false.
bool df::Object::isSolid() const {
	return solidness != df::SPECTRAL;
}

// Set object solidness, with checks for consistency.
// Return 0 if ok, else -1.
int df::Object::setSolidness(df::Solidness new_solid) {
	solidness = new_solid;
	// TODO Fix!
	return 0;
}

// Return object solidness.
df::Solidness df::Object::getSolidness() const {
	return solidness;
}



// Set Object sprite to new one.
// If set_box is true, set bounding box to size of Sprite.
// Set sprite index to 0 (first frame).
void df::Object::setSprite(Sprite *p_new_sprite, bool set_box) {
	p_sprite = p_new_sprite;
	sprite_index = 0;

	if(set_box) {
		box.setHorizontal(p_sprite->getWidth()-1);
		box.setVertical(p_sprite->getHeight()-1);
	}
}

// Return pointer to sprite associated with this Object.
df::Sprite *df::Object::getSprite() const {
	return p_sprite;
}

// Set Sprite to be centered at Object position (pos).
void df::Object::setCentered(bool centered) {
	sprite_center = centered;
}

// Indicates whether a sprite is centered at Object Position.
bool df::Object::isCentered() const {
	return sprite_center;
}

// Set index of current Sprite frame to be displayed.
void df::Object::setSpriteIndex(int index) {
	if( index >= 0  && (p_sprite == NULL || p_sprite->getFrameCount() > index)) {
		sprite_index = index;
	} else {
		LOG("Attempting to set illegal sprite index!");
	}
}

// Get index of current Sprite frame to be displayed.
int df::Object::getSpriteIndex() const {
	return sprite_index;
}

// SLows down sprite animations.
// Sprite slowdown is in multiples of GameManager frame time.
void df::Object::setSpriteSlowdown(int new_slowdown) {
	sprite_slowdown = new_slowdown;
}
int df::Object::getSpriteSlowdown() const {
	return sprite_slowdown;
}
void df::Object::setSpriteSlowdownCount(int new_slowdown_count) {
	sprite_slowdown_count = new_slowdown_count;
}
int df::Object::getSpriteSlowdownCount() const {
	return sprite_slowdown_count;
}

// Draws the object to the back buffer
void df::Object::draw() {
	if(p_sprite == NULL) {
		return;
	}

	// Ask graphics manager to draw current frame.
	GraphicsManager::getInstance().drawFrame(pos,
		p_sprite->getFrame(getSpriteIndex()), sprite_center, p_sprite->getColor());

	if(getSpriteSlowdown() == 0) {
		return; // If sprite isn't animated.
	}
	int count = getSpriteSlowdownCount()+1;

	if(count >= getSpriteSlowdown()) {
		count = 0;
		sprite_index++;

		// If at last frame, loop to beginning.
		if(sprite_index >= p_sprite->getFrameCount()) {
			sprite_index = 0;
		}
	}

	setSpriteSlowdownCount(count);
}

// Set Object's bounding box.
void df::Object::setBox(df::Box new_box) {
	box = new_box;
}

// Get Object's bounding box.
df::Box df::Object::getBox() const {
	return box;
}
