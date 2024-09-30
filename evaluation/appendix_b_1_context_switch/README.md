# context-measure

Estimate the duration of a context switch.

# Build
```
$ cmake -Bbuild .
$ make -C build
```

# Run

Open two terminals.

In the first terminal, start the process to switch *from*. This process constantly writes a time stamp counter into shared memory.

```
$ build/context-measure-from
```

In the second terminal, start the process to switch *into*.
There are two variants:
- `build/context-measure-to-clear` does not set the `prefetch_disable` bit. This process can be used to measure a transition between two processes that do not have the `prefetch_disable` bit set. This binary can also be used on a stock kernel.
- `build/context-measure-to-set` sets the `prefetch_disable` just after it was started. This process can be used to measure a transition from a process with the bit cleared (the `from` process) to one with the bit set. This binary cannot be used on a stock kernel.

These processes run until they collect a specified number of samples (by default 10,000). The samples are printed to stdout and should be redirected into a log file through the shell. Debug output is printed to stderr.

Example: 
```
$ build/context-measure-to-clear > out_kernel_pf_clear.log
```

`plot.py` generates Figure 8 in the paper.