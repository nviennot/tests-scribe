#include <scribe.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>

int main(int argc, char **argv)
{
	char new_hostname[4096];
	char old_hostname[4096];
	struct utsname uts;

	if (scribe_is_replaying()) {
		int rand;
		int frand;

		/*
		 * We want to change the hostname, to change the real uname to
		 * validate the data copy.
		 */
		scribe_disable();

		gethostname(old_hostname, sizeof(old_hostname));

		frand = open("/dev/urandom", O_RDONLY);
		read(frand, &rand, sizeof(rand));
		close(frand);
		sprintf(new_hostname, "scribe_test_hostname_%08X", rand);
		sethostname(new_hostname, strlen(new_hostname));
		scribe_enable();
	}
	clear_regs();

	uname(&uts);

	if (scribe_is_replaying()) {
		scribe_disable();
		sethostname(old_hostname, strlen(old_hostname));
		scribe_enable();
	}
	clear_regs();

	write(1, &uts, sizeof(uts));

	return 0;
}
