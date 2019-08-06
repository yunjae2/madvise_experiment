# SEQUENTIAL EXPERIMENT

### EXPERIMENT
If a region is applied MADV_SEQUENTIAL, will it take same time to be applied
regardless of the size of region? Because current implementation in Linux,
madvise() only sets VM_SEQ_READ flag in the corresponding vma and returns.
Also, VM_SEQ_READ is used for page cache and idle page tracking, does behavior
of anon pages changes or not? It should not be changing, refer to the
Implementation.
