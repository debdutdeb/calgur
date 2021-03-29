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
	char *file_name = mutant_string(
			"'file://", filename, "'", NULL);

	char *cmd = mutant_string(
			"gsettings set org.gnome.desktop.background picture-uri ", file_name, NULL);

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

	char *filename = mutant_string(
			getenv ("HOME"), "/.cache", ptr, NULL);

	if (!download (curl, url, filename)) {
		fprintf (stderr, "Image download failed. URL: %s\n", url);
		return false;
	}

	bool success = set_wall_from_file (filename);
	free (filename);
	return success;
}

