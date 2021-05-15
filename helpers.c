#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "helpers.h"

extern int errno;

int run_command(const char *cmd) {
	FILE *pipe = popen(cmd, "r");
	if (!pipe) {
		perror("pipe");
		return -1;
	}
	for (int c = fgetc(pipe); c != EOF; c = fgetc(pipe))
		putchar(c);
	return 0;
}

bool download(CURL *curl, const char *url, const char *filename)
{

	FILE *fs = fopen (filename, "wb");
	if (!fs) {
		perror ("fopen");
		return false;
	}

	bool status = true;

	curl_easy_setopt (curl, CURLOPT_URL, url);
	curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt (curl, CURLOPT_WRITEDATA, fs);
	curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1l);

	CURLcode cc = curl_easy_perform(curl);

	if (CURLE_OK != cc) {
		perror ("curl_easy_perform");
		status = false;
	}
		
	fclose (fs);
	curl_easy_reset (curl);
	return status;
}

static size_t __write_callback
(void *data, size_t size, size_t nmemb, void *userp)
{
	size_t totalsize = size * nmemb;
	CURLdata *mem = (CURLdata *) userp;

	char *ptr = realloc (mem->data, mem->size + totalsize + 1);
	if (!ptr) {
		perror ("out of memory");
		return -1;
	}
	mem->data = ptr;
	memcpy (mem->data + mem->size, data, totalsize);
	mem->size += totalsize;
	memset (mem->data + mem->size, 0, 1);

	return totalsize;
}

CURLdata *get_response (CURL *curl, char *url)
{
	CURLdata *mem = malloc (sizeof (CURLdata));
	mem->data = malloc (1); 
	mem->size = 0;
	if (!mem->data) {
		perror ("malloc");
		return NULL;
	}

	curl_easy_setopt (curl, CURLOPT_URL, url);
	curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, &__write_callback);
	curl_easy_setopt (curl, CURLOPT_WRITEDATA, (void *) mem);
	curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1l);

	CURLcode cc = curl_easy_perform (curl);
	if (CURLE_OK != cc) {
		perror ("curl_easy_perform");
		return NULL;
	}

	return mem;
}

char *mutant_string (size_t length, const char *tokens[length])
{
    size_t total_size = 0;
    size_t sizes[length] ;
    for (int i = 0; i < length; i++) {
        sizes[i] = strlen(tokens[i]);
        total_size += sizes[i];
    }
    
    char *ptr = malloc (total_size + 1);
    if (!ptr) return NULL;
    *(ptr + total_size) = 0;

    for (int pos = 0, index = 0; index < length; index++)
    {
        memcpy(ptr + pos, tokens[index], sizes[index]);
        pos += sizes[index];
    }
    return ptr;
}

bool extension
(const char *__src, const char *__snip)
{
	char *ext = strrchr (__src, '.');
	if (!ext) return false;
	return (bool)
		strcmp (*__snip == '.' ? ext : ext+1, __snip) == 0;
}