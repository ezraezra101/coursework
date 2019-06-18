#include "EventCollision.h"
#include "EventOut.h"
#include "GraphicsManager.h"
#include "ObjectListIterator.h"
#include "WorldManager.h"
#include "utility.h"

df::WorldManager::WorldManager() {
	// It's a constructor!
	setType("WorldManager");

	setBoundary(
		df::Box(df::Position(0,0),
				df::WINDOW_HORIZONTAL_CHARS_DEFAULT,
				df::WINDOW_VERTICAL_CHARS_DEFAULT)
		);

}
df::WorldManager::WorldManager(const df::WorldManager &other) {
	setType("WorldManager");
}
void df::WorldManager::operator=(const df::WorldManager &other) {
	setType("WorldManager");
}

df::WorldManager::~WorldManager() {
	shutDown();
}

df::WorldManager &df::WorldManager::getInstance() {
	static df::WorldManager *world_manager = new df::WorldManager();
	return *world_manager;
}

// Initialize Game World to be empty
int df::WorldManager::startUp() {
	updates.clear();
	deletions.clear();
	return df::Manager::startUp();
}

// Delete all game world Objects
void df::WorldManager::shutDown() {
	ObjectList ol = updates; // Copy list so we can delete during iteration???
	ObjectListIterator li(&ol);
	for(li.first(); not li.isDone(); li.next()) {
		delete li.currentObject();
	}

	df::Manager::shutDown();
}

// Add Object to world. Return 0 if ok, else -1
int df::WorldManager::insertObject(df::Object *p_o) {
	return updates.insert(p_o);
}

// Remove Object from world. return 0 if ok, else -1
int df::WorldManager::removeObject(df::Object *p_o) {
	return updates.remove(p_o);
}

df::ObjectList df::WorldManager::getAllObjects() const {
	return updates;
}

void df::WorldManager::update() {

	// Update object positions mased on their velocities.
	df::ObjectListIterator to_move(&updates);
	while(not to_move.isDone()) {
		Object *p_o = to_move.currentObject();
		int x = p_o->getXVelocityStep();
		int y = p_o->getYVelocityStep();
		if(x || y) {
			df::Position pos(p_o->getPosition().getX() + x, p_o->getPosition().getY() + y);
			moveObject(to_move.currentObject(), pos);
		}
		to_move.next();
	}


	df::ObjectListIterator to_delete(&deletions);
	while(not to_delete.isDone()) {
		delete to_delete.currentObject(); // Destructor removes it from updates
		to_delete.next();
	}
	deletions.clear();
}

int df::WorldManager::markForDelete(df::Object *p_o) {
	// Prevent duplication the laziest way I know how: Remove it then add it.
	int success = deletions.remove(p_o) ? 0 : 1;
	return deletions.insert(p_o) || success;
}

// Draws all objects in the world
void df::WorldManager::draw() {
	df::ObjectListIterator iter(&updates);

	// Draw lower altitude objects before higher ones.
	for(int alt=0; alt < df::MAX_ALTITUDE; alt++) {
		iter.first();

		while(not iter.isDone()) {
			if(iter.currentObject()->getAltitude() == alt) {
				// Bounding box coordinates are relative to Object,
				// so convert to world coordinates.
				df::Box temp_box = getWorldBox(iter.currentObject());

				// Only draw if Object would be visible on window (intersects view).
				if(boxIntersectsBox(temp_box, view)) {
					iter.currentObject()->draw();
				}
			}
			iter.next();
		}
	}

}

// Return a list of Objects collided with at Position 'where'.
// Collisions only occur with solid objects.
// Does not consider whether p_o is solid or not.
df::ObjectList df::WorldManager::isCollision(df::Object *p_o, df::Position where) const {
	df::ObjectList collision_list; // Make empty list.
	// iterate through all objects.
	df::ObjectListIterator i(&updates);

	while(not i.isDone()) {
		df::Object *p_temp = i.currentObject();
		df::Box new_box = getWorldBox(p_o, where);

		// Same location and both solid?
		if( p_temp != p_o && p_temp->isSolid() &&
			df::boxIntersectsBox(getWorldBox(p_temp), new_box) ) {
			collision_list.insert(p_temp);
		}

		i.next();
	}

	return collision_list;
}

// Move Object.
// If no collision with solid, move ok, else don't move.
// If p_o is Spectral, move anyway
// Return 0 if move ok, else -1 if collision with solid.
int df::WorldManager::moveObject(df::Object *p_o, df::Position where) {
	if(p_o->isSolid()) { // Need to be solid for collisions
		// Get collisions
		ObjectList list = isCollision(p_o, where);
		if(not list.isEmpty()) {
			bool can_move = true;

			// Iterate over list
			ObjectListIterator i(&list);
			while(not i.isDone()) {
				df::Object *p_temp = i.currentObject();

				// Create collision event.
				df::EventCollision c(p_o, p_temp, where);

				// Send to both objects.
				p_o   ->eventHandler(&c);
				p_temp->eventHandler(&c);

				// If both HARD, then cannot move.
				if( p_o   ->getSolidness()==df::HARD &&
					p_temp->getSolidness()==df::HARD) {

					can_move = false;
				}

				i.next();
			}

			if(can_move == false)
				return -1;
		} // If collision ends
	}

	// If the object is inside the stage, but moving outside of it:
	bool p_o_in 	= p_o->getPosition().getX() >= 0 && p_o->getPosition().getX() < df::GraphicsManager::getInstance().getHorizontal() &&
					  p_o->getPosition().getY() >= 0 && p_o->getPosition().getY() < df::GraphicsManager::getInstance().getVertical();
	bool moving_out = where.getX() < 0 || where.getX() >= GraphicsManager::getInstance().getHorizontal() ||
					  where.getY() < 0 || where.getY() >= GraphicsManager::getInstance().getVertical();

	// If here, no collision between two HARD objects so allow move.
	p_o->setPosition(where);

	if(p_o_in && moving_out) {
		// Generate out of bounds event and send to Object.
		df::EventOut ov;
		p_o->eventHandler(&ov);
	}

	// If view is following this object, adjust view.
	if(p_view_following == p_o) {
		setViewPosition(p_o->getPosition());
	}

	return 0;
}

// Set game world boundary.
void df::WorldManager::setBoundary(df::Box new_boundary) {
	boundary = new_boundary;
}

// Get game world boundary.
df::Box df::WorldManager::getBoundary() const {
	return boundary;
}

// Set player view of game world.
void df::WorldManager::setView(df::Box new_view) {
	view = new_view;
}

// Get player view of game world.
df::Box df::WorldManager::getView() const {
	return view;
}

// Set view to center window on Position view_pos.
// View edge will not go beyond world boundary.
void df::WorldManager::setViewPosition(df::Position view_pos) {
	// Make sure horizontal not out of world boundary.
	int x = view_pos.getX() - view.getHorizontal() / 2; // View 
	if(x + view.getHorizontal() > boundary.getHorizontal()) {
		x = boundary.getHorizontal() - view.getHorizontal();
	}
	if(x < 0) {
		x = 0;
	}

	// Make sure vertical not out of world boundary.
	int y = view_pos.getY() - view.getVertical() / 2;
	if(y + view.getVertical() > boundary.getVertical()) {
		y = boundary.getVertical() - view.getVertical();
	}
	if(y < 0) {
		y = 0;
	}

	// Set view.
	view.setCorner(df::Position(x, y));
}

// Set view to center window on Object.
// Set to NULL to stop following.
// If p_new_view_following not legit, return -1, else 0.
int df::WorldManager::setViewFollowing(df::Object *p_new_view_following) {
	// Seet to NULL to turn 'off' following.
	if(p_new_view_following == NULL) {
		p_view_following = NULL;
		return 0;
	}

	// Iterate over all Objects, make sure p_new_view_following
	// is one of the Objects
	bool valid_object = false;
	df::ObjectListIterator i(&updates);
	while(not i.isDone()) {
		if(p_new_view_following == i.currentObject()) {
			valid_object = true;
			break;
		}

		i.next();
	}

	// If found, adjust attribute accordingly and set view position.
	if(valid_object) {
		p_view_following = p_new_view_following;
		setViewPosition(p_view_following->getPosition());
		return 0;
	} else {
		return -1;
	}
}

