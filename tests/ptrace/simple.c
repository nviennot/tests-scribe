#include <stdio.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>


void child_do_this()
{
	int i;
        int *p=NULL;

	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	sleep(1);
	*p = 1; /* Seg fault */
	
	return;
}

int main(int argc, char **argv)
{
	int pid, status;
	struct user_regs_struct regs = {0};
	if ((pid = fork())==0) {
		child_do_this(); /* the child never returns */
		return 0;
	}

	wait(&status);
	if (WIFSTOPPED(status))
		printf("child has stopped...\n");
	if (WIFEXITED(status))
		printf("child has exited...\n");
	printf("calling ptrace.\n");
	if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0)
		perror("");
	
        printf("eax: %lx, ebx: %lx, ecx: %lx, edx: %lx, esi: %lx\n", 
			regs.eax, 
			regs.ebx, 
			regs.ecx,
			regs.edx,
			regs.esi);

	return 0;
}
