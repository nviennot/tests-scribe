#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <scribe.h>

void sig_handler(int signum)
{
	printf("Got SIGPIPE\n");
}

void register_sig_handler()
{
	struct sigaction action;
	action.sa_handler = sig_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGPIPE, &action, NULL);
}

void do_sleep(void *arg)
{
	usleep(100000);
}

int main(int argc, char **argv)
{
	int pipe_fds[2];
	int rfd, wfd;
	int res;
	char buffer[100];
	register_sig_handler();

	pipe(pipe_fds);
	rfd = pipe_fds[0];
	wfd = pipe_fds[1];

	if (!fork()) {
		close(rfd);
		usleep(10000);
		res = write(wfd, "Hello ", strlen("Hello "));
		printf("write() = %d\n", res);
		write(wfd, "World", strlen("World"));
		printf("write() = %d\n", res);
		return 0;
	}
	close(wfd);
	scribe_on_replay(do_sleep, NULL);
	close(rfd);
	return 0;
}
