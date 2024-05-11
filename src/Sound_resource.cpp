#include <iostream>
#include <string>
#include "Sound_resource.h"
#include "Setting.h"

Sound_resource::Sound_resource()
{
	chunk = nullptr;
	path = "";
	played_once_only = false;
}

Sound_resource::~Sound_resource()
{
	if (chunk != nullptr) {
		Mix_FreeChunk(chunk);
		chunk = nullptr;
	}

	path = "";
}

bool Sound_resource::LoadFromFile(const char* file)
{
	// As only WAVs are supported, ".wav" is omitted from the filename and appended here
	path = Setting::sound_path + file + ".wav";

	// Load sound at specified path
	chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr) {
		std::cout << "Mix_LoadWAV Error: Couldn't open " << path << ". SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

void Sound_resource::PlaySound()
{
	if (chunk == nullptr) {
		std::cout << "Mix_PlayChannel Error: Couldn't play chunk for " << path << ". SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	else {
		if (Setting::play_sounds) {
			// Play chunk on the first free channel and don't loop it
			Mix_PlayChannel(-1, chunk, 0);
			played_once_only = true;
		}
	}
}

void Sound_resource::PlaySoundOnce() 
{
	if (played_once_only == false) {
		PlaySound();
	}
}