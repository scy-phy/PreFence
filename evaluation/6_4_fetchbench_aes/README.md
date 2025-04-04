# Mitigating the FetchBench AES Attack With PreFence

We build upon the [FetchBench AES attack](https://github.com/scy-phy/FetchBench) to show that PreFence successfully prevents an end-to-end prefetching-based attack.

We modified the code of the AES attack in the following way to make the attack more stable and reliable:
- We return from the AES library function early, right after the vulnerable memory accesses have been performed. This simplifies synchronization.
- We move attacker and victim into the same process (again, to simplify synchronization).
- We use the privileged RAMINDEX interface to read the cache state directly instead of using Flush+Flush. This improves the reliability of the cache side channel.
- We insert PreFence system calls before and after the vulnerable library function is called in victim code to apply our defense.

We note that these modifications only make the attacker stronger, not the victim. PreFence can still defend against this stronger attacker.

Follow the following steps to reproduce this experiment on a Raspberry Pi 4 (either with patched kernel or with unpatched kernel):

1. Clone the original FetchBench repository and check out commit c657517:
```bash
git clone https://github.com/scy-phy/FetchBench.git
cd FetchBench
git checkout c657517
```
2. Apply the patch `prefence.patch` provided in this directory:
```bash
git apply ../prefence.patch
```
3. Set up the environment and compile the experiment code:
```bash
cd fetchbench
bash setup.sh
cd aes-attack/lib/mbedtls
export CC=clang
export CXX=clang++
CFLAGS=-O0 DEBUG=1 SHARED=1 make -j4 no_test
cd ../../program
objdump -d ../lib/mbedtls/library/libmbedcrypto.so | python3 ./generate_aligned_code.py > src/aligned_code.hh
cmake -B build .
make -C build
```
4. Finally, run the attack using the provided runscript:
```bash
# run either with prefetching, i.e., with PreFence NOT applied (should show leakage)
python3 runscript_prefence.py with_prefetch
# or without prefetching, i.e., with PreFence applied (should not show leakage)
python3 runscript_prefence.py without_prefetch
```
5. We provide a plot script to plot the results (similar to Figure 6 in the paper):
```bash
python3 plot_prefence.py
```

## Measure the Overhead of PreFence on the AES Function

To measure the timing overhead of the unavailability of prefetching (and thus, of PreFence) on the vulnerable AES function `mbedtls_internal_aes_encrypt`, follow the following steps:

1. Reset your FetchBench repository to the original commit `c657517` (alternatively, you can also check out a fresh copy as explained in step 1 in the previous section):
```bash
git reset --hard c657517
git clean -fX
```
2. Apply the patch `prefence_timing.patch` provided in this directory:
```bash
git apply ../prefence_timing.patch
```
3. If you rebooted your system since you called `setup.sh` for the main experiment above, re-run it now:
```bash
bash setup.sh
```
4. Re-compile MbedTLS (for this experiment, we do not return from the library function early, so we need to re-compile):
```bash
cd aes-attack/lib/mbedtls
export CC=clang
export CXX=clang++
CFLAGS=-O0 DEBUG=1 SHARED=1 make -j4 no_test
cd ../..
```
5. Run the experiment:
```bash
cd measure_timing
cmake -B build .
make -C build
build/measure_timing 0 > out-with_prefetch.log
build/measure_timing 1 > out-without_prefetch.log
```
6. Use the provided plot script to generate a histogram and compute the medians of the two distributions (this figure is not presented in the paper):
```bash
python3 ./plot_timing.py
```