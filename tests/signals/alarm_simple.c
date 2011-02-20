#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int i;
	alarm(1);
	for (i = 0; 1; i++) { /* infinite loop */
		printf("i=%d\n", i);
	}
	/* after 1 second, the program is supposed to stop */
	return 0;
}
