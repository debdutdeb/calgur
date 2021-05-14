#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cJSON.h"
#include "helpers.h"
#include "wallpaper.h"
#include "calgur.h"

int main 
(int argc, char *argv[], char *envp[])
{
	CURL *curl = curl_easy_init();
	if (!curl) {
		perror ("curl_easy_init");
		return -1;
	}

	CURLdata *mem = get_response (curl, "https://www.reddit.com/r/MinimalWallpaper/.json");
	if (! mem) {
		perror("get_response");
	}

	cJSON *json = cJSON_ParseWithLength (mem->data, mem->size);
	if (!json) {
		perror("cJSON_ParseWithLength");
		return -1;
	}

	char *url = reddit_get_random_image_url(json);
	assert(url);

	puts(url);


	if (!set_wall_from_url (curl, url))
		exit (EXIT_FAILURE);

error:
	cJSON_Delete(json);
	curl_easy_cleanup (curl);
	
	return EXIT_SUCCESS;
}

