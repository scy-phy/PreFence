# syscall-measure

Estimate the duration of a system call to prctl.

# Build
```
$ cmake -Bbuild .
$ make -C build
```

# Run

This process will run until it collected a specified number of samples (by default 10,000). The samples are printed to stdout and should be redirected into a log file through the shell. Debug output is printed to stderr.

Example command:

```
$ build/syscall-measure > out.log
```

`plot.py` generates Figure 9 in the paper.