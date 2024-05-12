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

	// Load sound at given path
	chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr) {
		std::cout << Mix_GetError() << std::endl;
		return false;
	}

	return true;
}

void Sound_resource::PlaySound()
{
	if (chunk == nullptr) {
		std::cout << Mix_GetError() << std::endl;
	}
	else {
		Mix_PlayChannel(-1, chunk, 0);
		played_once_only = true;
	}
}

void Sound_resource::PlaySoundOnce() 
{
	if (played_once_only == false) {
		PlaySound();
	}
}