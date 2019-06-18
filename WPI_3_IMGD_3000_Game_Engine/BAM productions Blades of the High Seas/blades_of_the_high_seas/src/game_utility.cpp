
#include "WorldManager.h"
#include "ObjectListIterator.h"

#include "game_utility.h"

// Checks to see if there are any enemies between the two objects
bool detectEnemies(df::Object *p_o1, df::Object *p_o2) {
	if(p_o1 == NULL || p_o2 == NULL) {
		return true; // Silly default
	}
	df::WorldManager &wm = df::WorldManager::getInstance();

	df::Position start = p_o1->getPosition();
	df::Position end   = p_o2->getPosition();

	// Find the top left
	df::Position new_corner(
		start.getX() < end.getX() ? start.getX() : end.getX(),
		start.getY() < end.getY() ? start.getY() : end.getY()
		);

	df::Box box(new_corner, abs(start.getX() - end.getX()), abs(start.getY() - end.getY()));

	df::ObjectList objects = wm.getAllObjects();
	df::ObjectListIterator iter(&objects);

	while(!iter.isDone()) {
		df::Object *p_temp = iter.currentObject();
		if( p_temp->isSolid() &&
			df::boxIntersectsBox(getWorldBox(p_temp), box) ) {

			// Check if object is an enemy and isn't one of our objects
			if( p_o1 != p_temp && p_o2 != p_temp &&
				(p_temp->getType() == "Foe" || p_temp->getType() == "Corporal") ) {
				return true;
			}

		}

		iter.next();
	}
	return false;
}

// Returns the hero 'nearest' to the position given.
Hero *nearestHero(df::Position pos) {
	df::WorldManager &wm = df::WorldManager::getInstance();

	df::ObjectList objects = wm.getAllObjects();
	df::ObjectListIterator iter(&objects);

	Hero *nearest_hero = NULL;

	while(!iter.isDone()) {
		df::Object *p_temp = iter.currentObject();

		// Check if object is an enemy
		if(p_temp->getType() == "Hero") {
			if(nearest_hero == NULL) {
				nearest_hero = static_cast<Hero *> (p_temp);

			} else if(	abs(p_temp->getPosition().getX() - pos.getX())
					< abs(nearest_hero->getPosition().getX() - pos.getX()) ) {
				nearest_hero = static_cast<Hero *> (p_temp);
			}
		}

		iter.next();
	}
	return nearest_hero;
}

// Returns the nearest foe in direction dir to the given object
df::Object *nearestFoe(df::Object *p_o, FACING dir) {
	df::WorldManager &wm = df::WorldManager::getInstance();

	df::Position pos = p_o->getPosition();

	df::ObjectList objects = wm.getAllObjects();
	df::ObjectListIterator iter(&objects);

	df::Object *nearest_foe = NULL;

	while(!iter.isDone()) {
		df::Object *p_temp = iter.currentObject();

		// Check if object is an enemy
		if(p_temp->getType() == "Foe" || p_temp->getType() == "Corporal") {
			if(p_temp == p_o)
				break;

			// If object is in the wrong direction
			if(dir == left && p_temp->getPosition().getX() > pos.getX()) {
				break;
			} else if(dir != left && p_temp->getPosition().getX() < pos.getX()) {
				break;
			}

			if(nearest_foe == NULL) {
				nearest_foe = p_temp;

			} else if(	abs(p_temp->getPosition().getX() - pos.getX())
					< abs(nearest_foe->getPosition().getX() - pos.getX()) ) {
				nearest_foe = p_temp;
			}
		}

		iter.next();
	}
	return nearest_foe;
}
