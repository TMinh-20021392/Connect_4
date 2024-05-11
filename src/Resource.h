#pragma once
#include <SDL.h>

/**
 * Abstract class for resources to inherit
 */
class Resource
{
public:
	/**
	 * Loads a resource file into memory
	 *
	 * @param file Name of a resource file
	 *
	 * @return True if a file was loaded successfully
	 */
	virtual bool LoadFromFile(const char* file) = 0;
};