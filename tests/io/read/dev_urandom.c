#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	char buffer[100];
	int frand = open("/dev/urandom", O_RDONLY);
	read(frand, buffer, sizeof(buffer));
	write(1, buffer, sizeof(buffer));
	return 0;
}
