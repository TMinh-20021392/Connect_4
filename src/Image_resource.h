#pragma once
#include <SDL.h>
#include "Resource.h"

/**
 * Resource interface implementation for handling images
 */
class Image_resource : public Resource
{
public:
	Image_resource();
	~Image_resource();

	bool LoadFromFile(const char* file) override;

	/**
	 * Set image position
	 */
	void SetPosition(int x = 0, int y = 0);

	void SetPositionWithSize(int x = 0, int y = 0, int w = 0, int z = 0);
	/**
	 * Sets image position to horizontal and vertical centre of window
	 */
	void SetPositionCenter();

	/**
	 * Sets image position to area outside window so it's hidden
	 */
	void SetPositionHidden();

	/**
	 * Gets the X position of a texture
	 */
	int GetPositionX();

	/**
	 * Gets the Y position of a texture
	 */
	int GetPositionY();

	/**
	 * Renders the image to renderer
	 *
	 * @param clip_rect optional rectangle for sprite sheet clipping
	 */
	void Render(SDL_Rect* clip_rect = nullptr);

private:
	/// Pointer to image file stored as SDL_Texture
	SDL_Texture* texture;

	/// Image width excluding scale factor
	int width;
	
	/// Image height excluding scale factor
	int height;

	/// Current X axis position of image
	int position_x;

	/// Current Y axis position of image
	int position_y;
};
