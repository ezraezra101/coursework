#include "Level.h"

class LevelWin : public Level {
public:
	LevelWin();
	~LevelWin();

	void spawnFoes();
	void spawnHeroes();
};