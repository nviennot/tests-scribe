#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

void notified()
{
	printf("The process is notified.\n");
	return;
}

void *notifier(void *args)
{
	int i;
	for (i = 0; i < 50; i++)
		raise(SIGUSR1);
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread_notifier;

	signal(SIGUSR1, notified);
	pthread_create(&thread_notifier, NULL, notifier, NULL);
	pthread_join(thread_notifier, NULL);
	
	return 0;
}
