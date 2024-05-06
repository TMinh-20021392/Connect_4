#pragma once
#include <SDL.h>
#include "Resource.h"

/**
 * Resource interface implementation for handling image files
 */
class Image_resource : public Resource
{
public:
	Image_resource();
	~Image_resource();

	bool load_from_file(const char* file) override;

	/**
	 * Sets image position to be read by render()
	 *
	 * @param x Number of pixels from the left of the screen 
	 * @param y Number of pixels from the top of the screen
	 */
	void set_position(int x = 0, int y = 0);

	void set_position_with_size(int x = 0, int y = 0, int w = 0, int z = 0);
	/**
	 * Sets image position to horizontal and vertical centre of window
	 */
	void set_position_centered();

	/**
	 * Sets the position of an image to an area outside of window so it can not be seen
	 */
	void set_position_hidden();

	/**
	 * Gets the X position of a texture
	 *
	 * @return X axis value
	 */
	int get_position_x();

	/**
	 * Gets the Y position of a texture
	 *
	 * @return Y axis value
	 */
	int get_position_y();

	/**
	 * Renders the image to renderer
	 *
	 * @param clip_rect optional rectangle for sprite sheet clipping
	 */
	void render(SDL_Rect* clip_rect = nullptr);

private:
	/// Pointer to the image file stored as an SDL_Texture
	SDL_Texture* texture;

	/// Width of the image not including scale factor
	int width;
	
	/// Height of the image not including scale factor
	int height;

	/// Current X axis position of the image
	int position_x;

	/// Current Y axis position of the image
	int position_y;
};
