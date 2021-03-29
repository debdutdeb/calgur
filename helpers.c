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
#include "debug.h"

// Because I don't like how N - 1 looks
#define index(N) N-1

extern int errno;

int run_command (const char *cmd) {

	int argc = 1;

	char **argv;

	char *command = strdup (cmd);
	check_memory(command);

	char *token;

	for (token=strtok(command, " "), argv=malloc(sizeof(char *)); token && argv; token=strtok(NULL, " "), argv=realloc(argv, sizeof(char *) * argc))
		argv[index(argc++)] = strdup (token);
	argv[argc] = NULL;

	log_info("argc: %d\n", argc);

	pid_t cpid;
	int *wstatus = &errno;

	switch (cpid = fork()) {
		case -1:
			sentinel("fork");
		case  0:
			check(-1 == execvp (argv[0], argv), "execvp");
		default:
			check(cpid == waitpid (cpid, wstatus, WUNTRACED|WCONTINUED), "waitpid");
	}

error:
	command ? free (command) : NULL;
	if (*argv)
		for (int f = 0; f < argc; f++)
			free (argv[f]);
	argv ? free (argv) : NULL;
	return *wstatus;

}

bool download (CURL *curl, const char *url, const char *filename)
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

char *mutant_string (const char *s, ...) {

	if (!s) return NULL;

	va_list argv;
	const char *c;
	size_t total_len;

	total_len = strlen (s);
	va_start (argv, s);
	
	while ((c = va_arg (argv, const char *)))
		total_len += strlen (c);
	va_end (argv);

	char *buf = malloc (total_len + 1);
	if (!buf) return NULL;

	int x;
	size_t len = strlen (s);
	memcpy (buf, s, len);
	x = len;

	va_start (argv, s);
	while ((c = va_arg (argv, const char *)))
	{
		len = strlen (c);
		memcpy (&buf[x], c, len);
		x += len;
	}
	va_end (argv);
	buf[x] = 0;

	return buf;
}
