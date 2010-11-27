#include <unistd.h>
#include <fcntl.h>
#include <scribe.h>

int main(int argc, char **argv)
{
	char x;
	if (fork()) {
		if (scribe_is_replaying()) {
			scribe_disable();
			usleep(10);
			scribe_enable();
		}
	}

	int f = open("/etc/passwd", O_RDONLY);
	read(f, &x, 1);
	close(f);

	return 0;
}
