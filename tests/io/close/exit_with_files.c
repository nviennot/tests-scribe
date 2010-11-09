#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int frand = open("/etc/passwd", O_RDONLY);
	return 0;
}
