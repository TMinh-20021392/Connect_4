#include <iostream>
#include <string>
#include "Resource_manager.h"

// Define vectors to store pointers to instances of resource classes
std::vector<std::pair<std::string, Image_resource*>> Resource_manager::images;
std::vector<std::pair<std::string, Sound_resource*>> Resource_manager::sounds;

Image_resource* Resource_manager::GetImage(const char* file) {

	// Check each entry until we find one that matches file as file is being 
	// used an ID
	for (unsigned int i = 0; i < images.size(); i++) {
		if (images[i].first == file) {
			return images[i].second;
		}
	}

	std::cout << "Could not find resource: " << file << std::endl;
	return nullptr;
}

Sound_resource* Resource_manager::GetSound(const char* file) {

	// Check each entry until we find one that matches file as file is being used an ID
	for (unsigned int i = 0; i < sounds.size(); i++) {
		if (sounds[i].first == file) {
			return sounds[i].second;
		}
	}

	std::cout << "Could not file resource: " << file << std::endl;
	return nullptr;
}

Image_resource* Resource_manager::LoadImage(const char* file) {

	// Load a resource file, store the instance and then return it
	auto* image = new Image_resource();
	image->LoadFromFile(file);
	images.emplace(images.end(), file, image);
	return GetImage(file);
}

Sound_resource* Resource_manager::LoadSound(const char* file) {

	// Load a resource file, store the instance and then return it
	auto* sound = new Sound_resource();
	sound->LoadFromFile(file);
	sounds.emplace(sounds.end(), file, sound);
	return GetSound(file);
}

void Resource_manager::UnloadImage(const char* file) {

	// Check each entry until we find one that matches file as file is being used an ID
	for (unsigned int i = 0; i < images.size(); i++) {
		if (images[i].first == file) {

			// Delete instance since it was created with new
			delete images[i].second;
			images[i].second = nullptr;
			
			// Remove unneeded entry from vector
			images.erase(images.begin() + i);
		}
	}
}

void Resource_manager::UnloadSound(const char* file) {

	// Check each entry until we find one that matches file as file is being used an ID
	for (unsigned int i = 0; i < sounds.size(); i++) {
		if (sounds[i].first == file) {

			// Delete instance since it was created with new
			delete sounds[i].second;
			sounds[i].second = nullptr;

			// Remove unneeded entry from vector
			sounds.erase(sounds.begin() + i);
		}
	}
}
