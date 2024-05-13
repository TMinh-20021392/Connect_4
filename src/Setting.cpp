#include "Setting.h"

const char* Setting::game_name = "Connect Four";

// Set to nullptr to use the default icon. Be careful as images that are too big
// don't always load.
const char* Setting::icon_path = "images/icon16.bmp";

const std::string Setting::image_path = "images/";

const std::string Setting::sound_path = "Sound/";

// Set first element to -1 to disable colour keying
const int Setting::color_key[] = { 0x00, 0xFF, 0xFF };