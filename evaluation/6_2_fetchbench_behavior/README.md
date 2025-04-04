# Using FetchBench to Test Prefetcher Behavior When Disabled

We extended the [FetchBench framework](https://github.com/scy-phy/FetchBench) to test for the behavior of a stride prefetcher when it is disabled.

Our main addition is a test for the behavior of stride prefetchers when they are disabled: [`FetchBench/src/testcase_stride_strideexperiment.hh` (ll. 288-516)](FetchBench/src/testcase_stride_strideexperiment.hh) [`FetchBench/src/testcase_stride.hh` (ll. 905-1007)](FetchBench/src/testcase_stride.hh)

We also provide `mod-ctrlpf`, a kernel module that allows controlling the prefetchers on Intel processors and the ARM A72 through a low-overhead `ioctl` interface. We use this interface during testing in our modified FetchBench code.

Follow the following steps to reproduce this experiment:

1. Clone the original FetchBench repository and check out commit c657517:
```bash
git clone https://github.com/scy-phy/FetchBench.git
cd FetchBench
git checkout c657517
```
2. Apply the patch `fetchbench_disable_behavior.patch` provided in this directory:
```bash
git apply ../fetchbench_disable_behavior.patch
```
3. Set up prerequisites:
```bash
cd fetchbench
# For Intel
# - edit intel.sh, replace the base frequency (2.70GHz) with the base frequency of your system
vi init-scripts/elitebook-gentoo.sh
# - run intel.sh
bash init-scripts/elitebook-gentoo.sh
# For Raspberry Pi 4/ARM A72
bash init-scripts/rpi4-1.5GHz.sh
```
4. Finally, run the stride prefetcher tests:
```bash
# Running as root is only required on Intel
sudo build/fetchbench -t stride
```

FetchBench should produce a plot in the working directory visualizing the results.