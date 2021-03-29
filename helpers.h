#pragma once
#include <curl/curl.h>
#include <stdbool.h>

typedef struct {
	void *data;
	size_t size;
} CURLdata;

int run_command (const char*);
CURLdata *get_response (CURL*, char*);
bool download (CURL*, const char*, const char*);
char *mutant_string (const char *, ...);
