# Kernel Patch

- `kernel/` contains the actual kernel patches (for Intel x86\_64 and the Raspberry Pi 4) and documentation on how we patched a kernel.
	- On the default Raspberry Pi OS distribution, the prefetcher cannot be controlled by the kernel, because the respective control register is not configured accordingly. In order to allow (only!) the kernel to control the prefetcher, we provide updated armstubs in the `armstub/` directory that enable this functionality. These files need to be copied to the boot partition of the microSD card in order to use our kernel patch on the Raspberry Pi.
- `userspace-test/` contains a test program that can be run from userspace and that checks whether the patched kernel behaves as expected.
- `pfdisset/` contains a simple wrapper program that can be used to run any other program with prefetching permanently disabled. The tool works similar to `taskset`. It sets the `prefetch_disable` flag and then starts the target application.
