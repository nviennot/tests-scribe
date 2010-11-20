#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int c;
	read(10, &c, 1);
	read(2, &c, 1);
	return 0;
}
