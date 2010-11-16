#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void sig_handler(int signum)
{
	printf("Signal: %d\n", signum);
	fflush(NULL);
}

void register_sig_handler()
{
	struct sigaction action;
	action.sa_handler = sig_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &action, NULL);
}

int main(int argc, char **argv)
{
	pid_t pid = getpid();
	int i;

	if (!fork()) {
		for (i = 0; i < 100; i++)
			kill(pid, SIGUSR1);
		return 0;
	}

	struct timespec ts = { .tv_sec = 1, .tv_nsec = 0};
	struct timespec rem;
	register_sig_handler();
	nanosleep(&ts, NULL);

	printf("Remain: %d, %d\n", rem.tv_sec, rem.tv_nsec);

	return 0;
}
