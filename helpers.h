#pragma once
#include <curl/curl.h>
#include <stdbool.h>

#define cat(...) mutant_string( \
    sizeof((const char *[]) {__VA_ARGS__}) / sizeof(const char*), \
    (const char *[]) {__VA_ARGS__})

typedef struct {
	void *data;
	size_t size;
} CURLdata;

int run_command (const char*);
CURLdata *get_response (CURL*, char*);
bool download (CURL*, const char*, const char*);
char *mutant_string (size_t, const char *[]);
