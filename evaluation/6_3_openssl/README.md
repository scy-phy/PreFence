# OpenSSL experiment

In this folder, we replicate the prefetching-based side channel in the bignum implementation of OpenSSL, first reported by Shin et al.[^1].

The underlying reason for the side channel is the lookup table `SQR_tb` defined in `crypto/bn/bn_gf2m.c`. This table is used in the function `BN_GF2m_mod_sqr_arr` in the same file.
The lookup table is accessed at offsets that depend on the second input parameter to the function, `const BIGNUM *a`. The number `a` is sliced into fractions of 4 bit each. Each fraction is then used as an index into the lookup table. If the resulting accesses produce a regular access pattern, the prefetcher is activated.

We provide the program `victim-bn` (derived from [here](https://github.com/scy-phy/plumber/tree/main/matcher/dynamic/victim-bn)) that uses this function. This program can be configured (using the define `USE_COUNTERMEASURE`) to either set the prefetch-disable flag before the call to `BN_GF2m_mod_sqr_arr` or not.

We show that, for inputs that produce a strided pattern, the prefetcher is activated when our countermeasure is not used, while it is not activated when our countermeasure is used.

## Reproducing

### Compiling OpenSSL

We use OpenSSL 1.1.0g (same as in [^1]). Clone the OpenSSL repo into this directory and check out the correct version tag:

```
git clone https://github.com/openssl/openssl.git
cd openssl
git checkout OpenSSL_1_1_0g
```

Apply the [Perl glob workaround](https://github.com/wazuh/wazuh/issues/4054#issuecomment-553453743):
```
sed -i "s#'File::Glob' => qw/glob/;#'File::Glob' => qw/bsd_glob/;#g" test/recipes/90-test_fuzz.t
sed -i "s#'File::Glob' => qw/glob/;#'File::Glob' => qw/bsd_glob/;#g" test/recipes/80-test_ssl_new.t
sed -i "s#'File::Glob' => qw/glob/;#'File::Glob' => qw/bsd_glob/;#g" test/recipes/40-test_rehash.t
sed -i "s#'File::Glob' => qw/glob/;#'File::Glob' => qw/bsd_glob/;#g" test/build.info
sed -i "s#'File::Glob' => qw/glob/;#'File::Glob' => qw/bsd_glob/;#g" test/run_tests.pl
sed -i "s#'File::Glob' => qw/glob/;#'File::Glob' => qw/bsd_glob/;#g" util/process_docs.pl
sed -i "s#'File::Glob' => qw/glob/;#'File::Glob' => qw/bsd_glob/;#g" Configure
```

Next, run `Configure`.

**For Alpine/x86_64:** To [build this version of OpenSSL with musl](https://github.com/openssl/openssl/issues/7406) (which is the default libc on Alpine), add `no-async` to the `Configure` call; when using a standard glibc, this flag may not be required.
```
./Configure -g linux-x86_64 shared no-async
```

**For ARM/Raspberry Pi:** Configure OpenSSL as follows:
```
./Configure -g linux-aarch64 shared
```

Finally, build OpenSSL:
```
make -j4
```

At the end of the process, there should be a file called `libcrypto.so.1.1` in the root of the `openssl` directory.

### Compiling the experiment

Change into the `victim-bn/` directory:
```
cd victim-bn
```

First we need to find some offsets into the OpenSSL library binary that likely need to be changed. Those offsets are defined at the top of `src/victim-bn.c` and can be identified with objdump as documented in the code comments. In summary, run the following commands and copy the addresses:
```
$ objdump -D ../openssl/libcrypto.so.1.1  | grep "<BN_gcd>:"
00000000000b4d60 <BN_gcd>:
$ objdump -D ../openssl/libcrypto.so.1.1  | grep "<SQR_tb>:"
000000000020d100 <SQR_tb>:
```

Compile the experiment using the usual CMake tuple:
```
cmake -Bbuild .
make -C build
```

### Running the experiment

Then run the experiment:
```
build/victim-bn 02468ace 9 > results.log
```

- `02468ace` is the `BIGNUM` to pass to the vulnerable function. This one produces a stride access pattern.
- `9` is the cache line to probe for prefetching effects. You need to choose this parameter depending on the placement of `SQR_tb` in memory. In our case, `SQR_tb` ended up aligned to a cache-line boundary (0x20d100 mod 64 is 0). In this case, `SQR_tb` spans exactly two cache lines (in our counting, lines 7 and 8) and cache line 9 is the first line that may show prefetching effects. If `SQR_tb` is not aligned to a cache line boundary in your case, the lookup table likely spans across 3 cache lines. In that case, change this parameter to `10` to probe the next cache line.

The plot script `plot_exp_openssl.py` takes the `results.log` file. It generates the file `plot_exp_openssl_overhead.png`, which is Figure 5 in the paper.

[^1]: Youngjoo Shin, Hyung Chan Kim, Dokeun Kwon, Ji Hoon Jeong, and Junbeom Hur. *Unveiling Hardware-based Data Prefetcher, a Hidden Source of Information Leakage.* In: Proceedings of the ACM SIGSAC Conference on Computer and Communications Security (CCS). 2018. https://doi.org/10.1145/3243734.3243736
