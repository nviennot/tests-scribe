#include <sys/mman.h>
#include <pthread.h>

#define SIZE 100

unsigned char mem[SIZE];

struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000};

void *do_mlock(void *args)
{
	int i;
	for (i = 0; i < 50; i++) {
		if (mlock(mem, SIZE) < 0)
			perror("mlock");
		nanosleep(&ts, NULL);
	}
	return NULL;
}

void *do_munlock(void *args)
{
	int i;
	for (i = 0; i < 50; i++) {
		if (munlock(mem, SIZE) < 0)
			perror("mlock");
		nanosleep(&ts, NULL);
	}
	return NULL;
}


int main(int argc, char **argv)
{
	pthread_t tha, thb;

	pthread_create(&tha, NULL, do_mlock, NULL);
	pthread_create(&thb, NULL, do_munlock, NULL);
	pthread_join(tha,NULL);
	pthread_join(thb,NULL);
	
	return 0;
}
