#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2
#define PAGE_SIZE 4096

char buffer[2*PAGE_SIZE];

void *thread(void *arg)
{
	int n = (int)arg;
	int i;
	int j;

	for (j = 0; j < 10; j++) {
		printf("thread %d starts\n", n);
		for (i = 0; i < sizeof(buffer); i++)
			buffer[i] = n;
		printf("thread %d ends\n", n);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t threads[NUM_THREADS];
	int i;

	for (i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, thread, (void *)i);
	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	return 0;
}
