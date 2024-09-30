# Fork of FetchBench

We forked this directory from [the original FetchBench repo at c657517](https://github.com/scy-phy/FetchBench/tree/c657517348a15cd0eb3a790a89e50710e21a5d1b)

Our main additions:
- Additional tests for the behavior of prefetchers when they are disabled: [`fetchbench/src/testcase_stride_strideexperiment.hh` (ll. 288-516)](fetchbench/src/testcase_stride_strideexperiment.hh) [`fetchbench/src/testcase_stride.hh` (ll. 905-1007)](fetchbech/src/testcase_stride.hh)
- `mod-ctrlpf`: A kernel module that allows controlling the prefetchers on Intel processors and the ARM A72 through a low-overhead `ioctl` interface

We omit parts of the original repository that are not related to our project (`aes-attack`, `covert-channel`).

---

(original README.md below)

---

# FetchBench

Code artifact for the submission ["FetchBench: Systematic Identification and Characterization of Proprietary Prefetchers"](https://publications.cispa.saarland/3991/1/ccs23-fetchbench.pdf) to CCS2023.

This repository contains:

- `fetchbench/`: Our prefetcher identification and characterization framework (sections 3 and 4)
- `aes-attack/`:  Our first case study (section 5.1): leaking parts of an AES key through the Cortex-A72 SMS prefetcher
- `covert-channel/`: Our second case study (section 5.2): a covert channel between normal world and secure world based on the Cortex-A72 SMS prefetcher
