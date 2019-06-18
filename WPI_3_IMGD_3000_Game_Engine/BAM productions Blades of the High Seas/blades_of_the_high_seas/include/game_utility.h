#ifndef GAME_UTILITY_H
#define GAME_UTILITY_H

#include "Position.h"
#include "Hero.h"

// Checks to see if there are any enemies between the two objects
bool detectEnemies(df::Object *p_o1, df::Object *p_o2);

// Returns the hero 'nearest' to the position given.
Hero *nearestHero(df::Position pos);

// Returns the nearest foe in direction dir to the given object
df::Object *nearestFoe(df::Object *p_o, FACING dir);

inline int abs(int x) {
	return x > 0 ? x : -x;
}

#endif