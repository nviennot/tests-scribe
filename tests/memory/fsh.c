#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>

#define NUM_THREADS 50

typedef char page_type[4096];

page_type *page;


void *start(void *arg)
{
	int i;
	int n = (int)arg;
	page_type *p = page;

	for (i =0; i < NUM_THREADS; i++)
		p[i][0] = 0;

	printf("Thread %d writing at %p\n", n, (void*)(&page[n][10]));
	for (i = 0; i < 10000; i++)
		p[n][10]++;
	for (i = 0; i < 10000; i++) {
		volatile int X = p[n][10];
	}
	for (i =0; i < NUM_THREADS; i++) {
		volatile int X = p[NUM_THREADS-i-1][0];
		p[i][0] = 0;
	}
	printf("Thread %d done\n", n);

	return NULL;
}

void load()
{
	page = (typeof(page))mmap(NULL, NUM_THREADS*4096,
				  PROT_READ | PROT_WRITE,
				  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (page == MAP_FAILED) {
		perror("MAP_FAILED");
		exit(1);
	}
}

void show_mapping() {
	char buffer[4000];
	int map = open("/proc/self/maps", O_RDONLY);
	int c = read(map, buffer, 4000);
	write(0, buffer, c);
	close(map);
}

int main()
{
	int i;

	load();
	//show_mapping();
	printf("Spawning threads (addr=%p)\n", (void*)page);

	pthread_t pt[NUM_THREADS];

	for (i = 0; i < NUM_THREADS; i++)
		pthread_create(&pt[i], NULL, start, (void*)i);

	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(pt[i], NULL);

	return 0;
}
