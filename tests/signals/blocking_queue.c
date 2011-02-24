#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

struct listnode {
	int data;
	struct listnode *next;
	struct listnode *prev;
};
struct listhead {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct listnode head;
} ghead = {
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.cond = PTHREAD_COND_INITIALIZER,
	.head = {
		.data = 0,
		.next = &ghead.head,
		.prev = &ghead.head,
	},
};

void insert(struct listnode *node)
{
	if (!node)
		return;
	ghead.head.prev->next = node;
	node->prev = ghead.head.prev;
	node->next = &ghead.head;
	ghead.head.prev = node;
}
struct listnode *pop(void)
{
	struct listnode *node=NULL;
	if (ghead.head.next != &ghead.head) {
		node = ghead.head.next;
		ghead.head.next = node->next;
		node->next->prev = &ghead.head;
		node->next = node;
		node->prev = node;
	}
	return node;
}
inline int empty(void)
{
	return ghead.head.next == &ghead.head;
}

void *producer(void *args)
{
	int i;
	for (i = 0; i < 50; i++) {
		struct listnode *node = calloc(1, sizeof(struct listnode));
		if (!node)
			break;
		node->data = i;
		pthread_mutex_lock( &ghead.mutex );
		insert(node);
		pthread_cond_broadcast(&ghead.cond);
		pthread_mutex_unlock( &ghead.mutex );
	}
	return NULL;
}
void *consumer(void *args)
{
	int i;
	struct listnode *n=NULL;
	for (i = 0; i < 50; i++) {
		pthread_mutex_lock( &ghead.mutex );
		while (empty()) {
		      pthread_cond_wait(&ghead.cond, &ghead.mutex);
		}
		n = pop();
		if (n)
			printf("%d\n", n->data);
		pthread_mutex_unlock( &ghead.mutex );
		free(n); n=NULL;
	}
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t p, c;
	pthread_create(&p, NULL, producer, NULL);
	pthread_create(&c, NULL, consumer, NULL);
	pthread_join(p,NULL);
	pthread_join(c,NULL);
	return 0;
}

