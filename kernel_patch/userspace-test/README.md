# userspace-test

This program tests whether the patched kernel changes the prefetcher's state as expected.
In particular, it performs the following tests:
- Set the `prefetch_disable` through a system call. Verify that the flag is set and that the prefetcher is disabled
- Fork a new process and verify that the set flag propagates into the child process
- Migrate the process to another core and verify that the prefetcher is still disabled
- Clear the `prefetch_disable` flag. Verify that the flag is cleared and that the prefetcher is re-enabled

## Building
```
$ cmake -Bbuild .
$ make -C build
```

## Prerequisites

### Intel

Ensure that the `msr` kernel module is loaded before this program is started. This module is used to verify that the prefetcher's state by reading the respective MSRs.

Use `lsmod` to list the modules that are currently loaded and `modprobe` to load the module if it is not loaded.

```
$ lsmod | grep msr
# modprobe msr
```

### Raspberry Pi

Compile and load the kernel module in `rpi4-module-pfstate/`. This module allows a userspace process to query the current activation state, which we use for validation purposes in this test application.

```
$ cd rpi4-module-pfstate
$ make
$ sudo bash module_load.sh
```

## Running

### Intel

On Intel, the program needs to be run with root permissions (this is a neccessary to access the MSRs from userspace through the `msr` module). Remember that Alpine does not ship with `sudo` by default. Use `doas` instead.

```
$ doas build/pf-dis-user
```

### Raspberry Pi

On the Raspberry Pi, the program can be run without root permissions:

```
$ build/pf-dis-user
```
