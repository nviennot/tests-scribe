#include <unistd.h>
#include <stdio.h>
#include <asm/fcntl.h>

#define F_SETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 7)
#define F_GETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 8)

void __do_fcntl(int fd)
{
	int res1, res2, res3;

	res1 = fcntl(fd, F_GETFD);
	res2 = fcntl(fd, F_SETFD, ~res1);
	res3 = fcntl(fd, F_GETFD);
	printf("GETFD: %08x -> %08x -> %08x\n", res1, res2, res3);

	res1 = fcntl(fd, F_GETFL);
	res2 = fcntl(fd, F_SETFL, ~res1);
	res3 = fcntl(fd, F_GETFL);
	printf("GETFL: %08x -> %08x -> %08x\n", res1, res2, res3);

	res1 = fcntl(fd, F_GETPIPE_SZ);
	res2 = fcntl(fd, F_SETPIPE_SZ, res1+4096);
	res3 = fcntl(fd, F_GETPIPE_SZ);
	printf("GETPIPE_SZ: %08x -> %08x -> %08x\n", res1, res2, res3);
}

void do_fcntl(int *fds, int num_fds)
{
	int i;
	for (i = 0; i < num_fds; i++)
		__do_fcntl(fds[i]);
	__do_fcntl(1000);
}

int main(int argc, char **argv)
{
#define NUM_FDS 6
	int fds[NUM_FDS];

	fds[0] = 0;
	fds[1] = 1;
	fds[2] = open("/dev/urandom", O_RDONLY);
	fds[3] = open("/etc/passwd", O_RDONLY);
	pipe(&fds[4]);

	do_fcntl(fds, NUM_FDS);
	if (!fork())
		do_fcntl(fds, NUM_FDS);
	if (!fork())
		do_fcntl(fds, NUM_FDS);
	if (!fork())
		do_fcntl(fds, NUM_FDS);
	do_fcntl(fds, NUM_FDS);

	return 0;
}
