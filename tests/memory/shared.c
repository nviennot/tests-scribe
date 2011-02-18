#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/* The test cases is designed to be racy */
void write_memory(int shmid, unsigned int size)
{
	unsigned char *shmem = shmat(shmid, 0, 0);
	int i,j;
	
	for (j = 0; j < 50; j++) {
		for (i = 0; i < size; i++)
			shmem[i] = i+j;
	}
	return;
}

void read_memory(int shmid, unsigned int size)
{
	unsigned char *shmem = shmat(shmid, 0, 0);
	int i,j;
	for (j = 0; j < 50; j++) {
		for (i = 0; i < size; i++) {
			printf("%d", shmem[i]);
			if (i % 70)
				printf("\n");
		}
	}
	return;
}

int main(int argc, char **argv)
{
	unsigned int msize = 100;
	int mid = shmget(IPC_PRIVATE, msize, 0);
	
	if (!fork()) {
		write_memory(mid, msize);
	}
	read_memory(mid, msize);

	return 0;
}
