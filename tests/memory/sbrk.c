#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void *enlarge_memory(void *arg)
{
	int i;
	for (i = 0; i < 50; i++) {
		if (sbrk(1) < 0) {
			fprintf(stderr, "[enlarge] sbrk() failed\n");
		}
	}
	return NULL;
}

void *shrink_memory(void *arg)
{
	int i;
	for (i = 0; i < 50; i++) {
		if (sbrk(-1) < 0) {
			fprintf(stderr, "[shrink] sbrk() failed\n");
		}
	}
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t tha, thb;

	pthread_create( &tha, NULL, enlarge_memory, NULL);
	pthread_create( &thb, NULL, shrink_memory, NULL);

	pthread_join( tha, NULL );
	pthread_join( thb, NULL );

	return 0;
}
