#pragma once

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#define debug (M, ...)
#else
#define debug (M, ...) fprintf (stderr, "DEBUG %s:%d: " M "\n" \
		__FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define errno_message() (errno ? strerror (errno) : "None")

#define log_err(X, ...) fprintf (stderr, \
		"[ERROR] (%s:%d: errno: %s)" X "\n", __FILE__, __LINE__, \
		errno_message(), ##__VA_ARGS__)

#define log_warn(X, ...) fprintf (stderr, \
		"[WARNING] (%s:%d: errno: %s)" X "\n", __FILE__, __LINE__, \
		errno_message(), ##__VA_ARGS__)

#define log_info(X, ...) printf ("[INFO] (%s:%d) " X "\n", \
		__FILE__, __LINE__, ##__VA_ARGS__)

#define check(X, Y, ...) if (!(X)) { \
		log_err(Y, ##__VA_ARGS__); goto error; }

#define sentinel(X, ...) { log_err(X, ##__VA_ARGS__); \
		goto error; }

#define check_memory(P) check((P), "Out Of Memory.")
