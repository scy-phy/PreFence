# armstubs

To enable control over the prefetcher from EL1, we need to set bit 0 in ACTLR_EL2 to 1. We need EL2 privilege to write to this register. Thus, we have to do this before the kernel is loaded, as we are limited to EL1 afterwards.

Therefore we modify the so-called "armstubs", which set up the environment before the kernel is loaded, to set this bit. 

The directory `armstubs/` contains the modified source code of these stubs ([original source](https://github.com/raspberrypi/tools/tree/master/armstubs)) as well as the compiled armstub binaries.

# Usage

To use the armstubs on a Raspberry Pi, copy all `*.bin` files from `armstubs/` to the root directory of the boot partition. You can either copy these files to the SD-card off-line or copy them to `/boot` on a running system.

# Compiling

In case you want to re-compile the armstubs: just run `make` in `armstubs/`.
