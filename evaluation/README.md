# Evaluation

This folder contains small programs and scripts that we used to evaluate our implementation of PreFence.

- `6_2_fetchbench/` (Section 6.2): A fork of [FetchBench](https://github.com/scy-phy/FetchBench), extended with testcases to test for the behavior of prefetchers when they are disabled.
- `6_3_openssl/` (Section 6.3): Reproduces the attack first presented by Shin et al.; evaluates the effectiveness of PreFence
- `6_4_spec/`: Our SPEC benchmark configuration files and plot scripts to enable reproduction of our SPEC benchmark results
- `6_5_lighttpd/`: Our patch to lighttpd as well as two example web applications to measure the performance of PreFence in a real-world scenario

-----

- `appendix_b_1_context_switch/` (Appendix B.1): Measures the overhead of a context switch with PreFence enabled and disabled
- `appendix_b_2_syscall/` (Appendix B.2): Measures the overhead of the `prctl` system call to control the `prefetch_disable` bit from userspace
