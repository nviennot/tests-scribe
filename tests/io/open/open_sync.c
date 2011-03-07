#include <unistd.h>
#include <fcntl.h>
#include <scribe.h>

void do_sleep(void *arg)
{
	usleep(100000);
}

int main(int argc, char **argv)
{
	char x;
	if (fork())
		scribe_on_replay(do_sleep, NULL);

	int f = open("/etc/passwd", O_RDONLY);
	read(f, &x, 1);
	close(f);

	return 0;
}
