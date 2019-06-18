#ifndef __MUSIC_H__
#define __MUSIC_H__

// Library includes
#include <string>
#include <SFML/Audio.hpp>

namespace df{

class Music {
private:
	sf::Music music;				// The SFML music.
	std::string label;				// Text label to identify sound.

	Music(Music const &);
	void operator=(Music const&);
public:
	Music();

	// Load music from file.
	// Return 0 if ok, else -1.
	int loadMusic(std::string filename);

	// Set label associated with music.
	void setLabel(std::string new_label);

	// Get label associated with music.
	std::string getLabel() const;

	// Play music.
	// If loop is true, repeat play when done.
	void play(bool loop=false);

	// Stop music.
	void stop();

	// Pause music.
	void pause();

	// Return SFML music.
	sf::Music *getMusic();
};

}
#endif

