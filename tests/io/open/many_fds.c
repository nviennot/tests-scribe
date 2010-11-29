#include <unistd.h>
#include <fcntl.h>
#include <scribe.h>

int main(int argc, char **argv)
{
	int i;
	for (i = 0; i < 10000; i++)
		open("/etc/passwd", O_RDONLY);
	return 0;
}
