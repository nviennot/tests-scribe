#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void signaled()
{
	printf("The process is alarmed.\n");
	raise(SIGKILL);
}


int main(int argc, char **argv)
{
	int i;
	signal(SIGALRM, signaled);
	alarm(1);

	for (i = 0; 1; i++) {
		printf("%d\n", i);
	}
	return 0;
}
