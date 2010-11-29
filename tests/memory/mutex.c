#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>

#define NUM_THREADS 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int data[1000];

void *start(void *arg)
{
	int i;
	int n = (int)arg;

	pthread_mutex_lock(&mutex);
	for(i = n; i <= NUM_THREADS; i++)
		data[i] = n;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main()
{
	int i;
	pthread_t pt[NUM_THREADS];

	for (i = 0; i < NUM_THREADS; i++)
		pthread_create(&pt[i], NULL, start, (void*)i);
	
	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(pt[i], NULL);

	for (i = 0; i <= NUM_THREADS; i++)
		printf("%d", data[i]);
	printf("\n");
	
	return 0;
}
