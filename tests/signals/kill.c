#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define NUM_CHILDREN  10

/* The parent process fork children processes and kill them. */

int main(int argc, char **argv)
{
	int i;
	pid_t pid = 1;
	pid_t pids[NUM_CHILDREN] = {0};

	for (i = 0; i < NUM_CHILDREN; i++) {
		pid = fork();
		while (!pid)  /* child processes are spinning */
			printf(" ");
		pids[i] = pid;
	}

	/* parent kill the children and wait for them */
	for (i = 0; i < NUM_CHILDREN; i++) {
                printf("killing %d\n", pids[i]);
		kill(pids[i], SIGKILL);
		waitpid(pids[i]);
	}
	return 0;
}
