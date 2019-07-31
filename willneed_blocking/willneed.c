#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	long total_mem, test_mem;
	long size_a, size_b;
	char yn;
	char *a, *b;
	int err;

	int page_size = getpagesize();

	while (1) {
		printf("Total system memory (GiB): ");
		scanf("%ld", &total_mem);
		printf("Test memory (GiB): ");
		scanf("%ld", &test_mem);

		size_a = total_mem * 1024 * 1024 * 1024;
		size_b = test_mem * 1024 * 1024 * 1024;
		a = malloc(size_a);
		b = aligned_alloc(page_size, size_b);
		printf("Accessing array b..\n");
		memset(b, 0, size_b);
		printf("Accessing array a..\n");
		memset(a, 0, size_a);
		printf("Freeing array a..\n");
		free(a);

		printf("madvise? (y/n): ");
		scanf(" %c", &yn);
		if (yn == 'y') {
			if (err = madvise(b, size_b, MADV_WILLNEED)) {
				printf("madvise failed! errno: %d\n", err);
				printf("addr: %p\n", b);
				break;
			}
		}

		printf("quit? (y/n): ");
		scanf(" %c", &yn);
		if (yn == 'y')
			break;

		free(b);
	}
	return 0;
}
