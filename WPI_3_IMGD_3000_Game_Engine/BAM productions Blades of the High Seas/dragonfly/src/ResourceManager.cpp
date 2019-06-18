#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "Color.h"
#include "ResourceManager.h"
#include "LogManager.h"

static inline std::string getALine(std::ifstream &file, std::string &line) {
	std::string str;
	getline(file, str);
	// Remove ALL newlines!
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	line = str;
	return str;
}

static df::Color stringToColor(std::string color_string) {
	if(color_string == "black") {
		return df::BLACK;
	} else if(color_string == "red") {
		return df::RED;
	} else if(color_string == "green") {
		return df::GREEN;
	} else if(color_string == "yellow") {
		return df::YELLOW;
	} else if(color_string == "blue") {
		return df::BLUE;
	} else if(color_string == "magenta") {
		return df::MAGENTA;
	} else if(color_string == "cyan") {
		return df::CYAN;
	} else if(color_string == "white") {
		return df::WHITE;
	} else {
		return df::COLOR_DEFAULT;
	}
}

// Returns -1 if it isn't possible to get the integer.
static int getIntFromFile(std::ifstream &file, const std::string token) {
	static std::string line;

	if(file.good()) {
		getALine(file, line);

		std::size_t found = line.find(token);
		if (found==std::string::npos) {
  			LOG("Sprite file doesn't have a %s", token.c_str());
  			return -1;
  		}

  		return atoi(line.c_str() + token.length()+1);

	} else {
		LOG("Unable to read %s from sprite file", token.c_str());
		return -1;
	}
}

// Returns "" if the next line of the file doesn't contain the token.
static std::string getStringFromFile(std::ifstream &file, const std::string token) {
	static std::string line;

	if(file.good()) {
		getALine(file, line);
		std::size_t found = line.find(token);
		if (found==std::string::npos) {
  			LOG("Sprite file doesn't have a %s", token.c_str());
  			return "";
  		}

  		return line.substr(token.length()+1); // token,length()+1 to line.length()

	} else {
		LOG("Unable to read %s from sprite file", token.c_str());
		return "";
	}
}

// Helper function for reading in a sprite
// NULL indicates an error.
static df::Sprite *loadSpriteFile(std::string filename) {
	std::ifstream file(filename);

	int frames, width, height;
	df::Color color;

	LOG("Reading in %s sprite file.", filename.c_str());
	if(file.is_open()) {
		// Want to read header, then each frame, then end.

		// Read in frames count
		frames= getIntFromFile(file, df::FRAMES_TOKEN);
		// Read in width
		width = getIntFromFile(file, df::WIDTH_TOKEN);
		// Read in height
		height= getIntFromFile(file, df::HEIGHT_TOKEN);
		// Read in color
		std::string color_string = getStringFromFile(file, df::COLOR_TOKEN);

		color = stringToColor(color_string);

		if(frames == -1 || width == -1 || height == -1 || color_string == "") {
			LOG("Failed to load sprite because of messed up headers.");
			file.close();
			return NULL;
		}

		std::string line = "";
		df::Sprite *p_sprite = new df::Sprite(frames);
		p_sprite->setWidth(width);
		p_sprite->setHeight(height);
		p_sprite->setColor(color);

		// Read in frames
		int frame;
		for(frame = 0; frame < frames; frame++) {

			// Read in a single frame
			std::string frame_string = "";
			int i;
			for(i=0; i<height; i++) {
				if(!file.good()) {
					LOG("Reached end of sprite file %s when reading in frame %d, line %d", filename.c_str(), frame, i);
					file.close();
					delete p_sprite;
					return NULL;
				}
				getALine(file, line);
				
				frame_string += line;
			}
			p_sprite->addFrame(df::Frame(width, height, frame_string));

			// Read in end of frame line.
			if(!file.good()) {
				LOG("Reached end of sprite file %s after frame %d", filename.c_str(), frame);
				file.close();
				delete p_sprite;
				return NULL;
			}

			getALine(file, line);
			if(line != df::END_FRAME_TOKEN) {
				LOG("Failure to load sprite! Didn't have '%s' line at frame end. Instead had: '%s'", df::END_FRAME_TOKEN.c_str(), line.c_str());
				file.close();
				delete p_sprite;
				return NULL;
			}
		} // End of reading in frames.

		// EOF of frames
		if(!file.good()) {
			LOG("Failure to load file! No end of sprite token!");
			file.close();
			delete p_sprite;
			return NULL;
		}
		getALine(file, line);
		if(line != df::END_SPRITE_TOKEN) {
			LOG("Failure to load sprite! Didn't have '%s'", df::END_SPRITE_TOKEN.c_str());
			file.close();
			delete p_sprite;
			return NULL;
		}


		file.close(); // Close file when done.

		return p_sprite;
	} else {
		return NULL; // File failed to open.
	}
}


// Preventing access to non-singleton functions
df::ResourceManager::ResourceManager() {
	setType("ResourceManager");

}
df::ResourceManager::ResourceManager(ResourceManager const &) {}
void df::ResourceManager::operator=(ResourceManager const &) {}

// Get the one and only instance of the ResourceManager
df::ResourceManager &df::ResourceManager::getInstance() {
	static ResourceManager resource_manager = df::ResourceManager();
	return resource_manager;
}

df::ResourceManager::~ResourceManager() {
	if(isStarted()) {
		shutDown();
	}
}

// Get ResourceManager ready to manage resources.
// Return 0 if OK, else a negative number.
int df::ResourceManager::startUp() {
	LOG("Resource manager starting up!");
	sprite_count = 0;
	sound_count  = 0;
	music_count  = 0;
	if(!df::LogManager::getInstance().isStarted()) {
		printf("Starting up Resource manager without log manager!\n");
	}

	return df::Manager::startUp();
}

// Shutdown manager, freeing any allocated sprites.
void df::ResourceManager::shutDown() {
	LOG("Resource manager shutting down!");
	int i;
	for(i=0; i<sprite_count; i++) {
		delete p_sprite[i];
	}
	for(i=0; i<sound_count; i++) {
		delete p_sound[i];
	}
	for(i=0; i<music_count; i++) {
		delete p_music[i];
	}

	df::Manager::shutDown();
}

// Load sprite from file
// Assign indicated label to sprite.
// Return 0 if ok, -1 if not.
int df::ResourceManager::loadSprite(std::string filename, std::string label) {
	if(sprite_count >= MAX_SPRITES) {
		LOG("ResourceManager is full of sprites (%d)!", MAX_SPRITES);
		return -1;
	}

	// Check for duplicate labels.
	int i;
	for(i=0; i<sprite_count; i++) {
		if(p_sprite[i]->getLabel() == label) {
			LOG("Tried to load a sprite with the same label (%s) as an already loaded sprite!", label.c_str());
			return -1;
		}
	}

	df::Sprite *sprite = loadSpriteFile(filename);
	if(sprite == NULL) {
		LOG("ResourceManager unable to load sprite!");
		return -1;
	}

	sprite->setLabel(label);

	p_sprite[sprite_count] = sprite;
	sprite_count++;

	return 0;
}

// Unload Sprite with indicated label
// Return 0 if ok, -1 if not.
int df::ResourceManager::unloadSprite(std::string label) {
	int i;
	for(i=0; i<sprite_count; i++) {
		// If sprite is in list:
		if(p_sprite[i]->getLabel() == label) {
			LOG("Deleting sprite %s", label.c_str());
			delete p_sprite[i]; // Delete the sprite
			// And move all the later sprites to fill the gap
			int j;
			for(j=i+1; j<sprite_count; j++) {
				p_sprite[j-1] = p_sprite[j];
			}
			sprite_count--;

			return 0;
		}
	}

	return -1;
}

// Find sprite with indicated label.
// Return pointer to it if found, else NULL.
df::Sprite *df::ResourceManager::getSprite(std::string label) const {
	int i;
	for(i=0; i<sprite_count; i++) {
		if(p_sprite[i]->getLabel() == label) {
			return p_sprite[i];
		}
	}

	return NULL;
}

// Load sound from file.
// Return 0 if ok, else -1.
int df::ResourceManager::loadSound(std::string filename, std::string label) {
	if(sound_count == MAX_SOUNDS) {
		LOG("Trying to load one too many sounds.");
		return -1;
	}
	// Check for duplicate labels.
	int i;
	for(i=0; i<sound_count; i++) {
		if(p_sound[i]->getLabel() == label) {
			LOG("Tried to load a sound with the same label (%s) as an already loaded sound!", label.c_str());
			return -1;
		}
	}

	if(not (p_sound[sound_count] = new df::Sound())) {
		LOG("Unable to create new Sound object.");
		return -1;
	}
	if(p_sound[sound_count]->loadSound(filename) == -1) {
		delete p_sound[sound_count];
		LOG("Failed to load sound!");
		return -1;
	}
	p_sound[sound_count]->setLabel(label);
	sound_count++;
	return 0;
}

// Return Sound with indicated label.
// Return 0 if ok, else -1.
int df::ResourceManager::unloadSound(std::string label) {
	int i;
	for(i=0; i < sound_count; i++) {
		if(label == p_sound[i]->getLabel()) {
			delete p_sound[i];
			// Scoot over remaining sounds
			int j;
			for(j=i; j<sound_count-1; j++) {
				p_sound[j] = p_sound[j+1];
			}
			p_sound[j+1] = NULL;
			sound_count--;
			return 0;
		}
	}

	return -1;
}

// Find Sound with indicated label.
// Return pointer to it if found, else NULL.
df::Sound *df::ResourceManager::getSound(std::string label) {
	int i;
	for(i=0; i<sound_count; i++) {
		if(label == p_sound[i]->getLabel()) {
			return p_sound[i];
		}
	}

	return NULL;
}

// Load music from file.
// Return 0 if ok, else -1.
int df::ResourceManager::loadMusic(std::string filename, std::string label) {
	if(music_count == MAX_MUSICS) {
		LOG("Trying to load one too many pieces of music.");
		return -1;
	}
	// Check for duplicate labels.
	int i;
	for(i=0; i<music_count; i++) {
		if(p_music[i]->getLabel() == label) {
			LOG("Tried to load a music file with the same label (%s) as an already loaded music!", label.c_str());
			return -1;
		}
	}
	if(not (p_music[music_count] = new df::Music())) {
		LOG("Unable to create new Music object.");
		return -1;
	}
	if(p_music[music_count]->loadMusic(filename) == -1) {
		LOG("Failed to load music!");
		delete p_music[music_count];
		return -1;
	}
	p_music[music_count]->setLabel(label);
	music_count++;
	return 0;
}

// Return Music with indicated label.
// Return 0 if ok, else -1.
int df::ResourceManager::unloadMusic(std::string label) {
	int i;
	for(i=0; i < music_count; i++) {
		if(label == p_music[i]->getLabel()) {
			delete p_music[i];
			// Scoot over remaining music
			int j;
			for(j=i; j < music_count-1; j++) {
				p_music[j] = p_music[j+1];
			}
			p_sound[j+1] = NULL;
			music_count--;
			return 0;
		}
	}

	return -1;
}

// Find Music with indicated label.
// Return pointer to it if found, else NULL.
df::Music *df::ResourceManager::getMusic(std::string label) {
	int i;
	for(i=0; i<music_count; i++) {
		if(label == p_music[i]->getLabel()) {
			return p_music[i];
		}
	}
	return NULL;
}

