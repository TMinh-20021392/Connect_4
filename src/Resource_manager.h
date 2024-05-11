#pragma once
#include <vector>
#include "Image_resource.h"
#include "Sound_resource.h"

/**
 * Manages game resource files such as images and sounds
 */
class Resource_manager
{
public:
	/**
	 * Returns a loaded image
	 *
	 * @param file Resource file to return
	 * @return Image resource
	 */
	static Image_resource* GetImage(const char* file);

	/**
	 * Returns a loaded sound
	 *
	 * @param file Resource file to return
	 * @return Sound resource
	 */
	static Sound_resource* GetSound(const char* file);

	/**
	 * Loads an image and stores it
	 *
	 * @param file Resource file to load
	 * @return Image resource
	 */
	static Image_resource* LoadImage(const char* file);

	/**
	 * Loads a sound and stores it
	 *
	 * @param file Resource file to load
	 * @return Sound resource
	 */
	static Sound_resource* LoadSound(const char* file);

	/**
	 * Deletes a image
	 *
	 * @param file Resource file to unload
	 */
	static void UnloadImage(const char* file);

	/**
	 * Deletes a sound
	 *
	 * @param file Resource file to unload
	 */
	static void UnloadSound(const char* file);

private:
	/// Stores pointers to instances of an Image_resource
	static std::vector<std::pair<std::string, Image_resource*>> images;

	/// Stores pointers to instances of an Sound_resource
	static std::vector<std::pair<std::string, Sound_resource*>> sounds;
};