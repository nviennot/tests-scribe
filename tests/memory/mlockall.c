#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>

#define SIZE 100

unsigned char mem[SIZE];

struct timespec ts = { .tv_sec = 0, .tv_nsec = 1};

void *worker(void *args)
{
	int i;
	for (i = 0; i < SIZE; i++) {
		mem[i] = i;
		nanosleep(&ts, NULL);
	}
	return NULL;
}



int main(int argc, char **argv)
{
	int i;
	pthread_t tha;

	mlockall(MCL_CURRENT);
	pthread_create(&tha, NULL, worker, NULL);
	for (i = 0; i < SIZE; i++) {
		printf("value: %d\n", mem[i]);
		nanosleep(&ts, NULL);
	}
	pthread_join(tha,NULL);
	munlockall();
	
	return 0;
}
