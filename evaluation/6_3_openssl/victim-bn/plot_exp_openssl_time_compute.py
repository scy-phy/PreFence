import sys
import os
import re
from dataclasses import dataclass
from pathlib import Path
import matplotlib.pyplot as plt
import statistics

@dataclass
class Dataset:
    use_countermeasure_flag: bool
    misses: list[int]
    times_compute: list[int]
    times_reload: list[int]

    def label(self) -> str:
        assert self.use_countermeasure_flag is not None
        return "set" if self.use_countermeasure_flag is True else "clear"

    def no_repetitions(self) -> int:
        assert len(self.times_compute) == len(self.times_reload)
        return len(self.times_compute)

if len(sys.argv) > 1 and os.path.isfile(sys.argv[1]):
    logfile = sys.argv[1]
else:
    logfile = "results.log"

datasets = {}
print(f"Parsing {logfile}")
with open(logfile) as file:
    dataset_set = Dataset(
        use_countermeasure_flag=True,
        misses=[],
        times_compute=[],
        times_reload=[],
    )
    dataset_clear = Dataset(
        use_countermeasure_flag=False,
        misses=[],
        times_compute=[],
        times_reload=[],
    )
    for line in file:
        rx_logline = re.match(r"^L;(?P<use_cm_flag>[01]);(?P<misses>-?\d+);(?P<time_compute>\d+);(?P<time_reload>\d+)$", line)
        if rx_logline is not None:
            use_cm_flag_intvalue = int(rx_logline.group("use_cm_flag"))
            use_cm_flag = True if use_cm_flag_intvalue != 0 else False
            if use_cm_flag == True:
                dataset_set.misses.append(int(rx_logline.group("misses")))
                dataset_set.times_compute.append(int(rx_logline.group("time_compute")))
                dataset_set.times_reload.append(int(rx_logline.group("time_reload")))
            else:
                dataset_clear.misses.append(int(rx_logline.group("misses")))
                dataset_clear.times_compute.append(int(rx_logline.group("time_compute")))
                dataset_clear.times_reload.append(int(rx_logline.group("time_reload")))
            continue
        print(f"Skipping: {line.strip()}")

    datasets["set"] = dataset_set
    datasets["clear"] = dataset_clear
    for dataset in datasets.values():
        assert len(dataset.times_reload) > 0
        assert len(dataset.times_compute) == len(dataset.times_reload)

fig = plt.gcf()
fig.set_size_inches(8, 5)

plt.rcParams["font.family"] = "Nimbus Roman"
plt.rcParams["font.size"] = 12

plt.xlabel("time")
plt.ylabel("number of observations")

hist_min = 300
hist_max = 700
plt.hist(
    x=datasets["set"].times_compute,
    color="red",
    alpha=0.5,
    range=(hist_min, hist_max),
    bins=hist_max-hist_min,
    label="bit set (PF disabled)",
)
plt.hist(
    x=datasets["clear"].times_compute,
    color="blue",
    range=(hist_min, hist_max),
    bins=hist_max-hist_min,
    alpha=0.5,
    label="bit cleared (PF enabled)",
)
plt.legend()
plt.savefig(
    f"plot_exp_openssl_time_compute_{Path(logfile).stem}.png",
    bbox_inches="tight", dpi=300
)

median_set = statistics.median(datasets['set'].times_compute)
median_clear = statistics.median(datasets['clear'].times_compute)
median_delta = (median_set - median_clear) / median_clear
print(f"median(set)   = {median_set}")
print(f"median(clear) = {median_clear}")
print(f"set is {median_delta * 100:6.2f} % slower than clear.")