# MADV_DONTNEED EXPERIMENT

### REQUIREMENTS
MADV_FREE flag is available since Linux 4.5, and the operation is changed after
Linux 4.12.

### EXPERIMENT
If a memory region is applied MADV_FREE, is it accessible? Under memory
pressure, is it still accessible?

### RESULT
Under memory pressure, the region is accessible but contains corrupt data.
Under no memory pressure, the region is accessible as well and holds the same
data.
