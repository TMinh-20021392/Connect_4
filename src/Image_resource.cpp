#include <iostream>
#include <string>
#include "Image_resource.h"
#include "Game.h"
#include "Setting.h"

Image_resource::Image_resource()
{
	texture = nullptr;
	width = 0;
	height = 0;
	position_x = 0;
	position_y = 0;
}

Image_resource::~Image_resource()
{
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}

bool Image_resource::LoadFromFile(const char* file)
{
	std::string path = Setting::image_path + file + ".bmp";

	// Load image at given path
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	if (surface == nullptr) {
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Colour key image if enabled in Settings.h
	if (Setting::color_key[0] != -1) {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, Setting::color_key[0], Setting::color_key[1], Setting::color_key[2]));
	}

	// Create texture from surface pixels
	texture = SDL_CreateTextureFromSurface(Game::GetRenderer(), surface);
	if (texture == nullptr) {
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Grab the non-scaled width and height 
	width = surface->w;
	height = surface->h;

	// Delete surface
	SDL_FreeSurface(surface);

	return true;
}

void Image_resource::SetPosition(int x, int y) {

	// Update X and Y positions for render
	position_x = x;
	position_y = y;
}

void Image_resource::SetPositionWithSize(int x, int y, int w, int h) {

	// Update X and Y positions for render
	position_x = x;
	position_y = y;

	// Update width and height
	width = w;
	height = h;
}

void Image_resource::SetPositionCenter() {
	position_x = (Setting::window_width / 2) - (width / 2);
	position_y = (Setting::window_height / 2) - (height / 2);
}

void Image_resource::SetPositionHidden() {

	// Place images at negative width and height so they are rendered outside screen
	position_x = -width;
	position_y = -height;
}

int Image_resource::GetPositionX() {
	return position_x;
}

int Image_resource::GetPositionY() {
	return position_y;
}

void Image_resource::Render(SDL_Rect* clip_rect)
{
	// Create a rectangle that defines where the texture should be draw and at what size
	SDL_Rect dest_rect = {
		position_x * Setting::scale_factor,
		position_y * Setting::scale_factor,
		width * Setting::scale_factor,
		height * Setting::scale_factor
	};

	if (clip_rect != nullptr) {
		dest_rect.w = clip_rect->w * Setting::scale_factor;
		dest_rect.h = clip_rect->h * Setting::scale_factor;
	}
	SDL_RenderCopyEx(Game::GetRenderer(), texture, clip_rect, &dest_rect, 0.0, nullptr, SDL_FLIP_NONE);
}