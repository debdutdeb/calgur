#pragma once

#include "cJSON.h"

char *reddit_get_random_image_url (const cJSON *);
static cJSON *reddit_get_image_object_list (const cJSON *);
