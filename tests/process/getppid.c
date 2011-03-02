#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <scribe.h>

void do_sleep(void *arg)
{
	sleep(1);
}

main()
{
	int ppid, pid, cpid;

	pid = getpid();

	cpid = fork();
	if (cpid < 0) {
		perror("fork");
		exit(1);
	} else if (cpid) {
		sleep(1);
	} else {
		printf("calling getppid...\n");
		scribe_on_replay(do_sleep, NULL);
		ppid = getppid();
		printf("result %d\n", ppid);
		if (pid != ppid) {
			printf("Bad ppid (expect: %d got: %d)\n", pid, ppid);
			exit(1);
		} else {
			printf("Success\n");
		}
	}
	exit(0);
}

