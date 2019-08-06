#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void print_diff_time(struct timespec start, struct timespec end)
{
	long diff_nsec;

	diff_nsec = (end.tv_sec - start.tv_sec) * 1000 * 1000 * 1000;
	diff_nsec += end.tv_nsec - start.tv_nsec;

	printf("%ld", diff_nsec);
}

void exp_1(void)
{
	struct timespec start_time, end_time;
	long mem_limit;
	long size_a;
	char *a;
	int err;

	int page_size = getpagesize();

	printf("# Exp 1. madvise latency vs size\n");
	printf("Memory limit (GiB): ");
	scanf("%ld", &mem_limit);

	mem_limit *= 1024 * 1024 * 1024;

	for (size_a = page_size; size_a < mem_limit; size_a *= 2) {
		printf("%*ld\t", 10, size_a);
		a = aligned_alloc(page_size, size_a);
		clock_gettime(CLOCK_REALTIME, &start_time);
		if (err = madvise(a, size_a, MADV_SEQUENTIAL)) {
			printf("madvise failed! errno: %d\n", err);
			break;
		}
		clock_gettime(CLOCK_REALTIME, &end_time);
		print_diff_time(start_time, end_time);
		printf("\n");
		free(a);
	}
	printf("\n");
}

void exp_2(void)
{
	struct timespec start_time, end_time;
	long sys_mem, target_mem;
	char *sys, *target;
	long i;
	int err;

	int page_size = getpagesize();

	printf("# Exp 2. mem swap-in latency with vs withoud madvise\n");
	printf("System memory (GiB): ");
	scanf("%ld", &sys_mem);
	printf("Target memory (to allocate, GiB): ");
	scanf("%ld", &target_mem);

	sys_mem *= 1024 * 1024 * 1024;
	target_mem *= 1024 * 1024 * 1024;

	for (int do_advise = 0; do_advise < 2; do_advise++) {
		sys = malloc(sys_mem);
		target = aligned_alloc(page_size, target_mem);
		if (do_advise) {
			printf("2) With madvise\n");
			if (err = madvise(target, target_mem, MADV_SEQUENTIAL)) {
				printf("madvise failed! errno: %d\n", err);
				break;
			}
		}
		else {
			printf("1) Without madvise\n");
		}

		printf("Memory init..\n");
		memset(target, 2, target_mem);
		printf("Swapping target out..\n");
		memset(sys, 2, sys_mem);
		free(sys);

		printf("Accessing target..\n");
		clock_gettime(CLOCK_REALTIME, &start_time);
		for (i = 0; i < target_mem; i++)
			target[i] = 1;
		clock_gettime(CLOCK_REALTIME, &end_time);
		printf("Latency: ");
		print_diff_time(start_time, end_time);
		printf("\n");
		free(target);
	}
}

int main(void)
{
	exp_1();
	exp_2();
}
