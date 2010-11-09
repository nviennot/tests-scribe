#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <inttypes.h>

uint64_t rdtsc() {
	uint32_t lo, hi;
	/* We cannot use "=A", since this would use %rax on x86_64 */
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	return (uint64_t)hi << 32 | lo;
}

int main(int argc, char **argv)
{
	printf("rdtsc() == %llu\n", (unsigned long long)rdtsc());
	return 0;
}
