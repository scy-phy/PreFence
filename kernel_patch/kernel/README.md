# PreFence Kernel Patch

This directory contains the kernel patches that adapt the scheduler code and the `prctl` system call to implement our countermeasure.

# Intel x86\_64 

We tested the patch on a Core i7-10510U CPU (Comet Lake) with [version 6.6.14](https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.6.14.tar.xz) of the Linux kernel.

The patch can manually be applied like this:
```
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.6.14.tar.xz
tar xf linux-6.6.14.tar.xz
cd linux-6.6.14
patch -p1 < ../patch-v4-6.6.14-0-lts-alpine-x86_64.patch
```

To run a patched kernel on bare-metal hardware, we installed Alpine Linux 3.19 and patched the kernel by following  the [Alpine documentation on how to build a custom kernel](https://wiki.alpinelinux.org/wiki/Custom_Kernel). We added our patch to the list of patches that the Alpine build system applies before compilation in the respective [APKBUILD](https://gitlab.alpinelinux.org/alpine/aports/-/blob/077bb3fbcdb019012691915c5a6d5d0cc39454db/main/linux-lts/APKBUILD) build script and built the kernel as per the instructions.

## Raspberry Pi 4 (aarch64/Cortex-A72)

We tested the patch with the [latest version of the Raspberry Pi kernel fork at the time of writing](https://github.com/raspberrypi/linux/tree/8a35f69cced9537584043aba6bf4d48434852aa2). Use the following commands to check out the kernel code and apply the patch:

```
mkdir linux-rpi
cd linux-rpi
git init
git remote add origin https://github.com/raspberrypi/linux.git
git fetch --depth 1 origin 8a35f69cced9537584043aba6bf4d48434852aa2
git checkout FETCH_HEAD
git apply <PreFence repo path>/kernel_patch/kernel/patch-v5-6.6.22-v8-rpi4-aarch64.patch
```

We then followed the official documentation to build the kernel. We chose to [cross-compile the kernel](https://www.raspberrypi.com/documentation/computers/linux_kernel.html#cross-compiling-the-kernel), but [building it locally on the Raspberry Pi](https://www.raspberrypi.com/documentation/computers/linux_kernel.html#building-the-kernel-locally) should also work.