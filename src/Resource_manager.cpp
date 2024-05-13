#include <iostream>
#include <string>
#include "Resource_manager.h"

// Vectors to store pointers to instances of resource classes
std::vector<std::pair<std::string, Image_resource*>> Resource_manager::images;
std::vector<std::pair<std::string, Sound_resource*>> Resource_manager::sounds;

Image_resource* Resource_manager::GetImage(const char* file) {

	// Check each entry until we find specified file
	for (unsigned int i = 0; i < images.size(); i++) {
		if (images[i].first == file) {
			return images[i].second;
		}
	}

	std::cout << "Could not find " << file << std::endl;
	return nullptr;
}

Sound_resource* Resource_manager::GetSound(const char* file) {

	// Check each entry until we find specified file
	for (unsigned int i = 0; i < sounds.size(); i++) {
		if (sounds[i].first == file) {
			return sounds[i].second;
		}
	}

	std::cout << "Could not find " << file << std::endl;
	return nullptr;
}

Image_resource* Resource_manager::LoadImage(const char* file) {

	// Load, store in vector then return file
	auto* image = new Image_resource();
	image->LoadFromFile(file);
	images.emplace(images.end(), file, image);
	return GetImage(file);
}

Sound_resource* Resource_manager::LoadSound(const char* file) {

	// Load, store in vector then return file
	auto* sound = new Sound_resource();
	sound->LoadFromFile(file);
	sounds.emplace(sounds.end(), file, sound);
	return GetSound(file);
}

void Resource_manager::UnloadImage(const char* file) {

	// Check each entry until we find specified file
	for (unsigned int i = 0; i < images.size(); i++) {
		if (images[i].first == file) {
			delete images[i].second;
			images[i].second = nullptr;
			
			// Remove from vector
			images.erase(images.begin() + i);
		}
	}
}

void Resource_manager::UnloadSound(const char* file) {

	// Check each entry until we find specified file
	for (unsigned int i = 0; i < sounds.size(); i++) {
		if (sounds[i].first == file) {
			delete sounds[i].second;
			sounds[i].second = nullptr;

			// Remove from vector
			sounds.erase(sounds.begin() + i);
		}
	}
}
