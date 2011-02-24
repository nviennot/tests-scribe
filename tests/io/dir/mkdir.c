#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000};

#define DIRNAME "/tmp/scribe-test-mkdir"

void *do_mkdir(void *args)
{
	int i;
	for (i = 0; i < 50; i++) {
		if (mkdir(DIRNAME, S_IWUSR | S_IWGRP | S_IWOTH) < 0)
			perror("mkdir");
		nanosleep(&ts, NULL);
	}
	return NULL;
}

void *do_rmdir(void *args)
{
	int i;
	for (i = 0; i < 50; i++) {
		if (rmdir(DIRNAME) < 0)
			perror("rmdir");
		nanosleep(&ts, NULL);
	}

	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t tha, thb;

	pthread_create(&tha, NULL, do_mkdir, NULL);
	pthread_create(&thb, NULL, do_rmdir, NULL);
	pthread_join(tha, NULL);
	pthread_join(thb, NULL);
	if (rmdir(DIRNAME) < 0)
		perror("rmdir");
	return 0;
}
