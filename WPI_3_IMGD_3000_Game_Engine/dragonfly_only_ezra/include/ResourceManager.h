#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <string>

#include "Manager.h"
#include "Sprite.h"
#include "Sound.h"
#include "Music.h"

namespace df {

// Maximum number of unique assets in game.
const int MAX_SPRITES = 1000;

// Delimiters used to parse Sprite files -
// The ResourceManager 'knows' the file format.
const std::string FRAMES_TOKEN = "frames";
const std::string HEIGHT_TOKEN = "height";
const std::string WIDTH_TOKEN  = "width";
const std::string COLOR_TOKEN  = "color";
const std::string END_FRAME_TOKEN  = "end";
const std::string END_SPRITE_TOKEN = "eof";
const int MAX_SOUNDS = 128;
const int MAX_MUSICS = 128;

class ResourceManager : public Manager {
private:
	Sprite *p_sprite[MAX_SPRITES];	// Array of sprites.
	int sprite_count;				// Number of loaded sprites
	Sound *p_sound[MAX_SOUNDS];
	int sound_count;
	Music *p_music[MAX_MUSICS];
	int music_count;

	// Preventing access to non-singleton functions
	ResourceManager();
	ResourceManager(ResourceManager const &);
	void operator=(ResourceManager const &);



public:
	
	// Get the one and only instance of the ResourceManager
	static ResourceManager &getInstance();

	~ResourceManager();

	// Get ResourceManager ready to manage resources.
	// Return 0 if OK, else a negative number.
	int startUp();

	// Shutdown manager, freeing any allocated sprites.
	void shutDown();

	// Load sprite from file
	// Assign indicated label to sprite.
	// Return 0 if ok, -1 if not.
	int loadSprite(std::string filename, std::string label);

	// Unload Sprite with indicated label
	// Return 0 if ok, -1 if not.
	int unloadSprite(std::string label);

	// Find sprite with indicated label.
	// Return pointer to it if found, else NULL.
	Sprite *getSprite(std::string label) const;

	// Load sound from file.
	// Return 0 if ok, else -1.
	int loadSound(std::string filename, std::string label);

	// Return Sound with indicated label.
	// Return 0 if ok, else -1.
	int unloadSound(std::string label);

	// Find Sound with indicated label.
	// Return pointer to it if found, else NULL.
	Sound *getSound(std::string label);

	// Load music from file.
	// Return 0 if ok, else -1.
	int loadMusic(std::string filename, std::string label);

	// Return Music with indicated label.
	// Return 0 if ok, else -1.
	int unloadMusic(std::string label);

	// Find Music with indicated label.
	// Return pointer to it if found, else NULL.
	Music *getMusic(std::string label);

};

}

#endif

