# PreFence

This repository contains the code artifact for the paper "A Scheduling-Aware Defense Against Prefetching-Based Side-Channel Attacks", to appear on arXiv as preprint.

It comprises the following components:

- `kernel_patch/`: A patch for the Linux kernel (v6.6.14). It extends the `prctl` system call to expose the `prefetch_disable` flag of a process to userspace and updates the scheduler to implement our countermeasure on Intel x86_64 CPUs.
- `evaluation/`: A collection of small test programs and scripts that we used to evaluate PreFence (as presented in Section 6 of the paper).
