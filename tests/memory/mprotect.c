#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function( void *ptr );

int touchme = 123;

void msleep(int millisec)
{
	usleep(millisec*1000);
}

main()
{
	int i, j;
	void* mem;
	char* page1, * page2;
	mem = mmap(NULL, 3*4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (mem == MAP_FAILED) {
		perror("MAP_FAILED");
		exit(1);
	}

	page1 = mem;
	page2 = mem+4096;

	printf("(P) bringing page1 in\n");
	page1[0] = 1;

	if (mprotect(page1, 4096, PROT_READ)) {
		perror("mprotect failed");
		exit(1);
	}
	if (mprotect(page1, 4096, PROT_READ|PROT_WRITE)) {
		perror("mprotect failed");
		exit(1);
	}


	printf("(P) page1 <- 'a'\n");
	page1[150] = 'a';

	if (!fork()) {
		printf("(C) bringing page2 in\n");
		page2[0] = 2;
		printf("(C) page2 <- 'b'\n");
		page2[150] = 'b';
		printf("(C) page1: '%c'\n", page1[150]);
		for (i = 0; i < 10; i++)
				page2[i] = i;
		msleep(100);
		printf("(C) ");
		for (i = 0; i < 10; i++)
			printf( "%02X ", page1[i]);
		printf("\n");
		exit(0);
	}
	for (i = 0; i < 10; i++)
			page1[i] = i;

	msleep(10);
	printf("(P) page2: '%c'\n", page2[150]);

	printf("(P) ");
	for (i = 0; i < 10; i++)
		printf( "%02X ", page2[i]);
	printf("\n");
	exit(0);
}

