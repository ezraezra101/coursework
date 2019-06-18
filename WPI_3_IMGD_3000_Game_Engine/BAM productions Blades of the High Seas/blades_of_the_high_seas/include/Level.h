#ifndef LEVEL_H
#define LEVEL_H

#include "Object.h"

#include "Hero.h"

const int LEVEL_POSITION = 4; // Level text Y position

class Level : public df::Object {
private:

	static Level *current_level;

	int level_number;
	std::string name;

	int alive_hero_count;
	int alive_foe_count;

	int level_countdown; // How many steps remaining until level starts.

	Hero *hero1;
	Hero *hero2;



protected:
	virtual void spawnHeroes();
	virtual void spawnFoes();

	void setFoeCount(int cnt);
	void setLevelCountdown(int cnt);

public:

	static Level *getCurrentLevel();

	Level();
	~Level();

	bool shouldSetGameOver();

	int eventHandler(const df::Event *p_e);

	void heroHasDied(Hero *h);
	void decrementFoeCount();

};

#endif