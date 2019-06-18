#ifndef __OBJECT_H__
#define __OBJECT_H__

// System includes
#include <string>

// Engine includes
#include "Box.h"
#include "Event.h"
#include "Position.h"
#include "Sprite.h"

namespace df {

enum Solidness {
	HARD,		// Object causes collisions and impedes.
	SOFT,		// Object causes collisions, but doesn't impede.
	SPECTRAL,	// Object doesn't cause collisions.
};


const std::string UNDEFINED_OBJECT = "df::undefined";

class Object {
private:
	int id;				// Unique object identifier.
	std::string type;	// User-defined identification.
	Position pos;		// Position in game world.
	int altitude; // 0 to MAX_ALTITUDE ('lower' objects drawn first) Default is MAX_ALTITUDE/2.

	float x_velocity;			// Hosizontal speed in spaces per step.
	float x_velocity_countdown; // Countdown to horizontal movement.
	float y_velocity;			// Vertical speed in spaces per step.
	float y_velocity_countdown;	// Countdown to vertical movement.
	Solidness solidness;

	Sprite *p_sprite;		// Sprite associated with object.
	bool sprite_center;		// True if sprite centered on object.
	int sprite_index;		// Current index frame for sprite.
	int sprite_slowdown;	// (1 = no slowdown, 0 = stop)
	int sprite_slowdown_count;	// Slowdown counter

	Box box;	// The Object's bounding box.

public:
	// Construct Object Set default parameters and
	// add to game world (WorldManager)
	Object();
	
	// Destroy Object
	// Remove from game world (WorldManager)
	virtual ~Object();

	// Set Object id.
	void setId(int new_id);

	// Get Object id.
	int getId() const;

	// Set type identifier of object.
	void setType(std::string new_type);

	// Get type identifier.
	std::string getType() const;

	// Set Position of Object.
	void setPosition(Position new_pos);

	// Get Position of Object.
	Position getPosition() const;

	// Set altitude of object with checks for range [0, MAX_ALTITUDE]
	// Return 0 if ok, -1 if not.
	int setAltitude(int new_altitude);

	// Return altitude of object
	int getAltitude() const;

	// Handle events (returns 1 if event is handled, 0 if not)
	virtual int eventHandler(const Event *p_e);

	// Get and set velocity (x,y).
	void setXVelocity(float new_x_velocity);
	float getXVelocity() const;
	void setYVelocity(float new_y_velocity);
	float getYVelocity() const;

	// Perform 1 step of velocity in horizontal direction.
	// Return horizontal distance moved this step.
	int getXVelocityStep();

	// Perform 1 step of velocity in vertical direction.
	// Return vertical distance moved this step.
	int getYVelocityStep();

	bool isSolid() const; // True if HARD or SOFT, else false.

	// Set object solidness, with checks for consistency.
	// Return 0 if ok, else -1.
	int setSolidness(Solidness new_solid);

	// Return object solidness.
	Solidness getSolidness() const;

	// Set Object sprite to new one.
	// If set_box is true, set bounding box to size of Sprite.
	// Set sprite index to 0 (first frame).
	void setSprite(Sprite *p_new_sprite, bool set_box=true);

	// Return pointer to sprite associated with this Object.
	Sprite *getSprite() const;

	// Set Sprite to be centered at Object position (pos).
	void setCentered(bool centered);

	// Indicates whether a sprite is centered at Object Position.
	bool isCentered() const;

	// Set index of current Sprite frame to be displayed.
	void setSpriteIndex(int index);

	// Get index of current Sprite frame to be displayed.
	int getSpriteIndex() const;

	// SLows down sprite animations.
	// Sprite slowdown is in multiples of GameManager frame time.
	void setSpriteSlowdown(int new_slowdown);
	int getSpriteSlowdown() const;
	void setSpriteSlowdownCount(int new_slowdown_count);
	int getSpriteSlowdownCount() const;

	// Draws the object to the back buffer
	virtual void draw();

	// Set Object's bounding box.
	void setBox(Box new_box);

	// Get Object's bounding box.
	Box getBox() const;

};

}
#endif
