#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	close(10);
	close(11);
	close(12);
	close(13);
	return 0;
}
