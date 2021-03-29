#include "common.h"
#include "debug.h"

extern int errno;

int run_command (const char *cmd) {

	int argc = 1;

	char **argv;

	char *command = strdup (cmd);
	check_memory(command);

	char *token;

	for (token=strtok(command, " "), argv=malloc(sizeof(char *)); token && argv; token=strtok(NULL, " "), argv=realloc(argv, sizeof(char *) * argc))
		argv[argc++ - 1] = strdup (token);
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
	if (command) free (command);
	if (*argv) {
		for (int f = 0; f < argc; f++) {
			log_info("%s", argv[f]);
			free (argv[f]);
		}
	}
	if (argv) free (argv);
	return *wstatus;

}

int main 
(int argc, char *argv[], char *envp[])
{
	printf ("Status Code: %d\n", run_command ("ls -al /root"));
	return 0;
}
