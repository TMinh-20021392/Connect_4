#pragma once
#include <SDL_mixer.h>
#include "Resource.h"

/**
 * Resource interface implementation for handling sound files
 */
class Sound_resource : public Resource
{
public:
	Sound_resource();
	~Sound_resource();

	bool LoadFromFile(const char* file) override;

	/**
	 * Plays the loaded sound file
	 */
	void PlaySound();

	/**
	 * Plays the loaded sound only once regardless of how many times this function is called, then never plays again
	 */
	void PlaySoundOnce();

private:
	/// Pointer to the sound file stored as a Mix_Chunk
	Mix_Chunk* chunk;

	/// Path to the loaded sound file
	std::string path;

	/// Flag mark a sound as having been played
	bool played_once_only = false;
};