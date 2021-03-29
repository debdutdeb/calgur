#pragma once

/* So I read the library that walgur
 * is using. I don't like it. Two reasons :
 * 1. I'm using C, I want to implement things natively, not using external commands.
 * 2. The code base seems messy. I'll just increase the dependency with feh
 * and use that for the wallpaper -_-
 */

#include <stdbool.h>
#include <curl/curl.h>


bool set_wall_from_file (const char*);
bool set_wall_from_url(CURL*, const char*);
