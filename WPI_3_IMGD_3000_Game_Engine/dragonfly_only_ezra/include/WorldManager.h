#ifndef __WORLD_MANAGER_H__
#define __WORLD_MANAGER_H__

// System includes
// #include <stdio.h>

// Engine includes
#include "Box.h"
#include "Manager.h"
#include "ObjectList.h"
#include "Object.h"
#include "utility.h"

namespace df {

const int MAX_ALTITUDE = 4;

class WorldManager : public Manager {
private:
	ObjectList updates;		// All the Objects in the world that needs updates
	ObjectList deletions;	// Objects in the world to delete.

	// It's a singleton class, so don't make these public.
	WorldManager();
	WorldManager(WorldManager const &);
	void operator=(WorldManager const&);


	Box boundary;
	Box view;
	Object *p_view_following;

public:
	~WorldManager();

	static WorldManager &getInstance();

	// Start up the Game world (initialize everything to empty)
	// Return 0.
	int startUp();

	// Shutdown game world (delete all game world Objects)
	void shutDown();

	// Add Object to world. Return 0 if ok, else -1
	int insertObject(Object *p_o);

	// Remove Object from world. Return 0 if ok, else -1.
	int removeObject(Object *p_o);

	// Return list of all Objects in world.
	ObjectList getAllObjects() const;

	// Update world.
	void update();

	// Indicate Object is to be deleted at the end of current game loop.
	// Return 0 if ok, else -1
	int markForDelete(Object *p_o);


	// Draw all objects
	void draw(void);

	// Return a list of Objects collided with at Position 'where'.
	// Collisions only occur with solid objects.
	// Does not consider whether p_o is solid or not.
	ObjectList isCollision(Object *p_o, Position where) const;

	// Move Object.
	// If no collision with solid, move ok, else don't move.
	// If p_o is Spectral, move anyway
	// Return 0 if move ok, else -1 if collision with solid.
	int moveObject(Object *p_o, Position where);


	// Set game world boundary.
	void setBoundary(Box new_boundary);

	// Get game world boundary.
	Box getBoundary() const;

	// Set player view of game world.
	void setView(Box new_view);

	// Get player view of game world.
	Box getView() const;

	// Set view to center window on Position view_pos.
	// View edge will not go beyond world boundary.
	void setViewPosition(Position view_pos);

	// Set view to center window on Object.
	// Set to NULL to stop following.
	// If p_new_view_following not legit, return -1, else 0.
	int setViewFollowing(Object *p_new_view_following);

};

}

#endif