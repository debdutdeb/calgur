#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "wallpaper.h"
#include "helpers.h"

extern int errno;

/* GNOME: gsettings set org.gnome.desktop.background picture-uri 'file://${ABSOLUTE PATH}'
 */

bool set_wall_from_file(const char *filename)
{
	char *file_name = cat(
			"'file://", filename, "'");

	char *cmd = cat(
			"gsettings set org.gnome.desktop.background picture-uri ", file_name);

	int code = run_command(cmd);
	bool status = true;

	if (code) {
		fprintf (
			stderr, "Wallpaper set failed. File: %s\n", file_name);
		fprintf (stderr, "Command: %s\n", cmd);
		status = false;
	}

	free (cmd);
	free (file_name);
	return status;
}


bool set_wall_from_url(CURL *curl, const char *url)
{

	char *ptr = strrchr(url, '/');
	if (!ptr) {
		fprintf (stderr, "Invalid URL.\n");
		return false;
	}

	char *filename = cat(
			getenv ("HOME"), "/.cache", ptr);
	assert(filename != NULL);
	puts(filename);

	if (!download (curl, url, filename)) {
		fprintf (stderr, "Image download failed. URL: %s\n", url);
		return false;
	}

	bool success = set_wall_from_file (filename);
	free (filename);
	return success;
}

