import gzip
import os
import re
import glob
import math
import statistics

# import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import binom

FILE_GLOB_WITH_PREFETCH = "./with_prefetch/out-with_prefetch-*.log.gz"
FILE_GLOB_WITHOUT_PREFETCH = "./without_prefetch/out-without_prefetch-*.log.gz"

### Parsing

def parse_file(path: str, expect_leakage: bool) -> int:
	# print(f"[{path:36s}] Parsing...")
	
	with gzip.open(path, "rb") as file:
		file.seek(-2048, os.SEEK_END)
		summary = file.read().decode()
	summary = summary[summary.find("Final Summary"):]

	result = [0 for _ in range(16)]
	no_correct_bits = 0
	no_0_bits = 0
	for line in summary.splitlines():
		rx = re.match(
			r"Key byte hypothesis +(?P<pos>\d+): "
			r"[01_]+ \((?P<found>[a-f0-9]{2})\), "
			r"correct: (?P<expected>[a-f0-9]{2}) \([E_]{2}\)",
			line
		)
		if rx is not None:
			pos = int(rx.group("pos"))
			found_nibble = int(rx.group("found"), 16) >> 4
			expected_nibble = int(rx.group("expected"), 16) >> 4
			no_correct_bits_local = (~(found_nibble ^ expected_nibble) & 0xf).bit_count()
			no_0_bits_local = ((~expected_nibble) & 0xf).bit_count()
			if (expect_leakage and no_correct_bits_local < 4):
				print(f"[{path:36s}] False Negative: {no_correct_bits_local}/4 (expected: 4)", line)
			elif (not expect_leakage and no_correct_bits_local == 4):
				print(f"[{path:36s}] False Positive: {no_correct_bits_local}/4 (expected: 0)", line)
			no_correct_bits += no_correct_bits_local
			no_0_bits += no_0_bits_local

	print(f"[{path:36s}] Recovered {no_correct_bits:3d}/128 bits (expected leakage: {str(expect_leakage):5s}).")
	return no_correct_bits, no_0_bits

results_with_prefetch_correct = []
for filepath in glob.glob(FILE_GLOB_WITH_PREFETCH):
	no_correct_bits, _ = parse_file(filepath, expect_leakage=True)
	results_with_prefetch_correct.append(no_correct_bits)

results_without_prefetch_correct = []
results_without_prefetch_0 = []
for filepath in glob.glob(FILE_GLOB_WITHOUT_PREFETCH):
	no_correct_bits, no_0_bits = parse_file(filepath, expect_leakage=True)
	results_without_prefetch_correct.append(no_correct_bits)
	results_without_prefetch_0.append(no_0_bits)

# DEBUG
# results_with_prefetch_correct =  [57, 53, 55, 64, 64, 64, 58, 54, 64, 51, 43, 59, 51, 60, 58, 51, 64, 61, 59, 59, 58, 56, 64, 48, 56, 55, 64, 64, 61, 64, 64, 54, 54, 50, 64, 61, 64, 57, 64, 63, 59, 60, 57, 59, 64, 58, 50, 64, 64, 64, 54, 64, 51, 59, 64, 64, 55, 55, 64, 59, 60, 49, 53, 64, 61, 64, 64, 55, 56, 60, 64, 55, 55, 59, 64, 64, 64, 64, 61, 61, 64, 53, 56, 58, 56, 61, 64, 64, 64, 49, 59, 55, 58, 56, 53, 54, 64, 64, 61, 58, 56, 64, 59, 58, 64, 52, 51, 58, 64, 61, 54, 64, 64, 58, 59, 54, 60, 64, 59, 60, 64, 55, 59, 57, 58, 64, 46, 58, 56, 64, 64, 58, 48, 49, 57, 54, 52, 64, 60, 64, 60, 41, 64, 52, 60, 64, 60, 64, 62, 64, 53, 58, 64, 45, 51, 64, 57, 64, 57, 64, 64, 53, 64, 58, 56, 57, 54, 57, 64, 59, 47, 58, 60, 58, 64, 60, 64, 57, 64, 58, 64, 51, 64, 64, 63, 47, 49, 64, 61, 64, 64, 64, 64, 55, 58, 57, 61, 64, 58, 56]
# results_without_prefetch_correct =  [28, 26, 29, 35, 27, 24, 33, 33, 31, 39, 40, 30, 26, 31, 34, 35, 37, 29, 27, 40, 30, 35, 38, 34, 28, 39, 29, 30, 29, 35, 35, 34, 30, 26, 37, 38, 27, 34, 28, 29, 30, 32, 35, 31, 40, 33, 26, 26, 25, 33, 30, 31, 28, 34, 33, 34, 30, 28, 34, 28, 32, 40, 34, 31, 38, 33, 34, 30, 34, 28, 33, 40, 35, 29, 31, 27, 30, 38, 33, 32, 32, 26, 35, 22, 29, 36, 35, 36, 31, 32, 36, 32, 31, 34, 35, 33, 35, 32, 31, 35, 30, 32, 29, 31, 36, 28, 30, 32, 32, 32, 30, 38, 33, 33, 38, 37, 27, 36, 34, 31, 30, 33, 28, 31, 35, 30, 27, 30, 31, 35, 32, 28, 23, 34, 33, 32, 33, 39, 41, 28, 30, 30, 31, 30, 32, 38, 33, 27, 31, 27, 37, 32, 30, 35, 33, 32, 32, 25, 32, 29, 29, 33, 32, 36, 34, 25, 38, 20, 24, 34, 29, 33, 32, 33, 21, 30, 29, 28, 28, 31, 32, 29, 35, 30, 39, 32, 32, 32, 31, 36, 31, 29, 31, 30, 29, 30, 39, 33, 25, 33]
# results_without_prefetch_0 =  [31, 26, 23, 32, 30, 29, 36, 35, 34, 37, 39, 30, 25, 31, 34, 37, 37, 25, 26, 40, 31, 39, 39, 34, 28, 39, 29, 35, 29, 35, 35, 34, 30, 26, 37, 39, 27, 34, 31, 29, 30, 32, 35, 35, 43, 37, 28, 26, 25, 33, 30, 30, 32, 34, 33, 34, 30, 30, 34, 28, 32, 36, 31, 35, 38, 31, 34, 30, 34, 31, 33, 40, 35, 29, 31, 33, 30, 32, 34, 32, 30, 25, 37, 22, 30, 38, 35, 36, 28, 36, 36, 32, 33, 34, 35, 32, 35, 31, 33, 35, 30, 33, 29, 31, 36, 28, 30, 31, 33, 32, 32, 38, 33, 33, 38, 37, 27, 36, 35, 31, 30, 33, 28, 31, 35, 30, 26, 30, 31, 34, 32, 28, 26, 35, 33, 32, 33, 37, 41, 32, 30, 30, 31, 30, 35, 38, 33, 27, 29, 25, 37, 30, 29, 34, 33, 34, 28, 22, 32, 29, 29, 37, 34, 37, 34, 23, 38, 23, 29, 36, 30, 31, 32, 33, 22, 27, 30, 32, 28, 33, 34, 27, 35, 28, 36, 31, 36, 35, 35, 36, 36, 31, 29, 30, 29, 30, 37, 35, 25, 34]

print("results_with_prefetch_correct = ", results_with_prefetch_correct)
print("results_without_prefetch_correct = ", results_without_prefetch_correct)
print("results_without_prefetch_0 = ", results_without_prefetch_0)

print("mean(results_with_prefetch_correct) = ", statistics.fmean(results_with_prefetch_correct))
print("mean(results_without_prefetch_correct) = ", statistics.fmean(results_without_prefetch_correct))
print("mean(results_without_prefetch_0) = ", statistics.fmean(results_without_prefetch_0))

print(
	"number of fully correct key recoveries (64 bits) in results_with_prefetch_correct",
	f"{results_with_prefetch_correct.count(64)}/{len(results_with_prefetch_correct)} =",
	results_with_prefetch_correct.count(64)/len(results_with_prefetch_correct)
)

### Compute binomial distribution
# number of independent bit guesses
bdist_n = 64
# probability of guessing one bit correctly
bdist_p = 0.5
# compute probability mass function
bdist_pmf_xvals = list(range(bdist_n + 1)) 
bdist_pmf_yvals = [binom.pmf(r, bdist_n, bdist_p) for r in bdist_pmf_xvals]

# #### Compute normal distribution (approximating the binomial distribution)
# # approximation of binomial distribution with normal distribution is only
# # allowed when the following conditions are met:
# assert (bdist_n * bdist_p > 5) and (bdist_n * (1 - bdist_p) > 5)
# # compute ND parameters
# ndist_mu = bdist_n * bdist_p # 32
# ndist_sigma = math.sqrt(bdist_n * bdist_p * (1 - bdist_p)) # 4
# ndist_scale = 1
# print("ndist_mu", ndist_mu)
# print("ndist_sigma", ndist_sigma)
# # compute function
# ndist_xvals = np.linspace(0, 64, 100)
# ndist_yvals = (
# 	ndist_scale *
# 	(1 / (ndist_sigma * np.sqrt(2 * np.pi))) *
# 	np.exp(-0.5 * (((ndist_xvals - ndist_mu) / ndist_sigma) ** 2))
# )

### Plotting

fig = plt.gcf()
fig.set_size_inches(5, 2)

plt.rcParams["font.family"] = "Nimbus Roman"
plt.rcParams["font.size"] = 12

plt.xlabel("number of matching bits")
plt.ylabel("relative frequency")

ax = plt.gca()
ax.hist(
    x=results_with_prefetch_correct,
    label="Without PreFence",
    bins=65,
    range=(0, 65),
    alpha=0.6,
    align="left",
    weights=[1/len(results_with_prefetch_correct) for _ in range(len(results_with_prefetch_correct))],
    color="#2ca02c", # green
)
ax.hist(
    x=results_without_prefetch_correct,
    label="With PreFence",
    bins=65,
    range=(0, 65),
    alpha=0.6,
    align="left",
    weights=[1/len(results_without_prefetch_correct) for _ in range(len(results_without_prefetch_correct))],
    color="#9467bd", # purple
)
# ax.hist(
#     x=results_without_prefetch_0,
#     label="0's in keys",
#     bins=65,
#     range=(0, 65),
#     alpha=0.6,
#     align="left",
#     weights=[1/len(results_without_prefetch_0) for _ in range(len(results_without_prefetch_0))],
#     color= "black",
# )
# # Plot normal distribution
# ax.plot(
# 	ndist_xvals,
# 	ndist_yvals,
# 	label=f"Random guessing",
# 	color="blue",
# 	alpha=0.5
# )
# Plot binomial distribution
ax.step(
	bdist_pmf_xvals,
	bdist_pmf_yvals,
	where="mid",
	color="red",
	label=f"Random guessing",
	alpha=0.5
)
ax.set_xticks([i for i in range(0, 65, 8)])
ax.legend(
	# ncol=2,
)
plt.savefig("plot_prefence.svg", bbox_inches="tight")

