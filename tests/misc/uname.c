#include <scribe.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>

#define BUFFER_SIZE 4096

void on_pre_replay(void *arg)
{
	char new_hostname[BUFFER_SIZE];
	char *old_hostname = arg;
	int rand;
	int frand;

	/*
	 * We want to change the hostname, to change the real uname to
	 * validate the data copy.
	 */
	gethostname(old_hostname, BUFFER_SIZE);

	frand = open("/dev/urandom", O_RDONLY);
	read(frand, &rand, sizeof(rand));
	close(frand);
	sprintf(new_hostname, "scribe_test_hostname_%08X", rand);
	sethostname(new_hostname, strlen(new_hostname));
}

void on_post_replay(void *arg)
{
	char *old_hostname = arg;
	sethostname(old_hostname, strlen(old_hostname));
}

int main(int argc, char **argv)
{
	char old_hostname[BUFFER_SIZE];
	struct utsname uts;

	scribe_on_replay(on_pre_replay, old_hostname);
	uname(&uts);
	scribe_on_replay(on_post_replay, old_hostname);

	write(1, &uts, sizeof(uts));

	return 0;
}
