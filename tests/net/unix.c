#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

int client(const char *addr, int abstract)
{
	struct sockaddr_un address;
	int  socket_fd, nbytes;
	char buffer[256];

	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(socket_fd < 0)
	{
		printf("socket() failed\n");
		return 1;
	}

	/* start with a clean address structure */
	memset(&address, 0, sizeof(struct sockaddr_un));

	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, UNIX_PATH_MAX, addr);
	if (abstract)
		address.sun_path[0] = 0;

	if(connect(socket_fd, 
		   (struct sockaddr *) &address, 
		   sizeof(struct sockaddr_un)) != 0)
	{
		printf("connect() failed\n");
		return 1;
	}

	nbytes = snprintf(buffer, 256, "hello from a client");
	nbytes = write(socket_fd, buffer, nbytes);

	nbytes = read(socket_fd, buffer, 256);
	if (nbytes < 0) {
		perror("(client) read failed\n");
		return 1;
	}
	buffer[nbytes] = 0;

	printf("MESSAGE FROM SERVER: %s\n", buffer);

	close(socket_fd);

	return 0;
}


int connection_handler(int connection_fd)
{
	int nbytes;
	char buffer[256];

	nbytes = read(connection_fd, buffer, 256);
	if (nbytes < 0) {
		perror("(client) read failed\n");
		return 1;
	}
	buffer[nbytes] = 0;

	printf("MESSAGE FROM CLIENT: %s\n", buffer);
	nbytes = snprintf(buffer, 256, "hello from the server");
	write(connection_fd, buffer, nbytes);

	close(connection_fd);
	return 0;
}

int server(const char *addr, int abstract)
{
	struct sockaddr_un address;
	int socket_fd, connection_fd;
	socklen_t address_length;
	pid_t child;

	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(socket_fd < 0)
	{
		printf("socket() failed\n");
		return 1;
	} 

	/* start with a clean address structure */
	memset(&address, 0, sizeof(struct sockaddr_un));

	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, UNIX_PATH_MAX, addr);
	if (abstract)
		address.sun_path[0] = 0;

	if(bind(socket_fd, 
		(struct sockaddr *) &address, 
		sizeof(struct sockaddr_un)) != 0)
	{
		printf("bind() failed\n");
		return 1;
	}

	if(listen(socket_fd, 5) != 0)
	{
		printf("listen() failed\n");
		return 1;
	}

	while((connection_fd = accept(socket_fd, NULL, NULL)) > -1)
	{
		child = fork();
		if(child == 0)
		{
			/* now inside newly created connection handling process */
			return connection_handler(connection_fd);
		}

		/* still inside server process */
		close(connection_fd);
	}

	close(socket_fd);
	return 0;
}

void do_test(const char *addr, int abstract)
{
	pid_t server_pid;

	server_pid = fork();
	if (!server_pid) {
		server(addr, abstract);
		exit(0);
	}
	usleep(100*1000);
	client(addr, abstract);
	usleep(100*1000);
	kill(server_pid, SIGTERM);
}

int main(void) {
	printf("Testing regular paths\n");
	do_test("/tmp/test.scribe.socket", 0);
	unlink("/tmp/test.scribe.socket");
	printf("Testing abstract paths\n");
	do_test("/tmp/test.scribe.socket", 1);
	return 0;
}
