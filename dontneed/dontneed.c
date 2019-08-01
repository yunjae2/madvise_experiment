#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
	int i, err;
	int nr_entries = 1000 * 1000;
	int *arr;

	int page_size = getpagesize();

	arr = aligned_alloc(page_size, nr_entries * sizeof(int));
	for (i = 0; i < nr_entries; i++) {
		arr[i] = i;
	}
	err = madvise(arr, nr_entries * sizeof(int), MADV_DONTNEED);
	if (err) {
		printf("madvise failed! errno: %d\n", err);
		printf("addr: %p\n", arr);
	}

	for (i = 0; i < nr_entries; i++) {
		if (arr[i] != i) {
			printf("corrupted! expected: %d, actual: %d\n", i, arr[i]);
			break;
		}
	}
	return 0;
}
