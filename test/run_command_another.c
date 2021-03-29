#include "common.h"

int run_command (const char *cmd) {

 	char **argv = (char **) malloc (sizeof(char *) * 2);
 	
 	*(argv++) = (char *) malloc (sizeof(char) * 10);
 	*(argv++) = (char *) malloc (sizeof(char) * 10);
 	
 	
 	int i = 2;
 	while (free(*--argv), --i);
 	free (argv);

	char *command[] = {
		"ls",
		"-al",
		NULL
	};
	
	pid_t cpid;
	int wstatus;
	
	switch (cpid = fork()) {
		case -1:
			perror ("fork");
			return -1;
		case  0:
			if (-1 == execvp (command[0], command)) {
				perror ("execvp");
				return -1;
			}
		default:
			if (cpid != waitpid (cpid, &wstatus, WUNTRACED|WCONTINUED))
			{
				perror ("waitpid");
				return -1;
			}
			return wstatus;
	}

}

int main 
(int argc, char *argv[], char *envp[])
{
	printf ("Status Code: %d\n", run_command ("ls -al"));
	return 0;
}
