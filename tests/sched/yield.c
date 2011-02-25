#include <stdio.h>
#include <sched.h>
#include <pthread.h>

void *worker(void *args)
{
	int i;
	for (i = 0; i < 50; i++) {
		if (pthread_yield() < 0)
			perror("pthread_yield");
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int i;
	pthread_t th_worker;

	pthread_create(&th_worker, NULL, worker, NULL);
	pthread_join(th_worker, NULL);
	return 0;
}
