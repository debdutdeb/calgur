#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "cJSON.h"
#include "calgur.h"
#include "helpers.h"

static cJSON *reddit_get_image_object_list (const cJSON *json)
{
	return cJSON_GetObjectItem(
			cJSON_GetObjectItem(json, "data"), "children");
}

char *reddit_get_random_image_url (const cJSON *json) {
	srand ((unsigned int) time (NULL));
	cJSON *images = reddit_get_image_object_list(json);
	char *url = cJSON_GetStringValue(
		cJSON_GetObjectItem(
		cJSON_GetObjectItem(cJSON_GetArrayItem(images, rand() % cJSON_GetArraySize(images)), "data"), "url"));
	if (extension(url, "jpg") || extension(url, "png"))
		return url;
	printf ("\"%s\" isn't an image url. Redoing..\n", url);
	return reddit_get_random_image_url (json);
}