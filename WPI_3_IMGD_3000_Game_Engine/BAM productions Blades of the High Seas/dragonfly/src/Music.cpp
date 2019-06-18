#include "Music.h"
#include "LogManager.h"

df::Music::Music() {

}

// These are PRIVATE
df::Music::Music(df::Music const &music) {
	// Don't do anything.
}
void df::Music::operator=(df::Music const &music) {
	// Don't do anything...
}

// Load music from file.
// Return 0 if ok, else -1.
int df::Music::loadMusic(std::string filename) {
	LOG("Loading music file %s", filename.c_str());
	if(music.openFromFile(filename) == false) {
		LOG("Failed to load %s", filename.c_str());
		return -1;
	}
	return 0;
}

// Set label associated with music.
void df::Music::setLabel(std::string new_label) {
	label = new_label;
}

// Get label associated with music.
std::string df::Music::getLabel() const {
	return label;
}

// Play music.
// If loop is true, repeat play when done.
void df::Music::play(bool loop) {
	music.setLoop(loop);
	music.play();
}

// Stop music.
void df::Music::stop() {
	music.stop();
}

// Pause music.
void df::Music::pause() {
	music.pause();
}

// Return SFML music.
sf::Music *df::Music::getMusic() {
	return &music;
}

