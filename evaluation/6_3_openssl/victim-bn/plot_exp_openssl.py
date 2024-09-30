import re
from dataclasses import dataclass
import matplotlib.pyplot as plt


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

logfiles = [
    "results.log",
]

datasets = []
for logfile in logfiles:
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

        for dataset in [dataset_set, dataset_clear]:
            assert len(dataset.times_reload) > 0
            assert len(dataset.times_compute) == len(dataset.times_reload)
            datasets.append(dataset)

fig = plt.gcf()
fig.set_size_inches(5, 1.2)

plt.rcParams["font.family"] = "Nimbus Roman"
plt.rcParams["font.size"] = 12

# plt.subplot(1, 2, 1)
plt.xlabel("time")
plt.ylabel("prefetch_disable")
plt.xlim(0, 1000)
plt.ylim(-0.65, 1.4)
boxplots = plt.boxplot(
    x=[ds.times_reload for ds in datasets],
    positions=[i for i in range(len(datasets))],
    showfliers=True,
    vert=False,
    widths=0.5,
)

for median_line in boxplots["medians"]:
    median = median_line.get_xdata()[0]
    ypos = median_line.get_ydata()[0]
    plt.text(
        x=median,
        y=ypos - 0.05,
        s=str(median),
        verticalalignment="top",
        horizontalalignment="center",
        color=median_line.get_color()
    )
ax = plt.gca()
ax.set_yticks([i for i in range(len(datasets))])
ax.set_yticklabels([ds.label() for ds in datasets])

plt.savefig("plot_exp_openssl_effective.png", bbox_inches="tight", dpi=300)

plt.clf() ##################################

fig = plt.gcf()
fig.set_size_inches(5, 1.2)

plt.rcParams["font.family"] = "Nimbus Roman"
plt.rcParams["font.size"] = 12

# plt.subplot(1, 2, 1)
plt.xlabel("time")
plt.ylabel("prefetch_disable")
plt.xlim(0, 1000)
plt.ylim(-0.65, 1.4)
boxplots = plt.boxplot(
    x=[ds.times_compute for ds in datasets],
    positions=[i for i in range(len(datasets))],
    showfliers=True,
    vert=False,
    widths=0.5,
)

for median_line in boxplots["medians"]:
    median = median_line.get_xdata()[0]
    ypos = median_line.get_ydata()[0]
    plt.text(
        x=median,
        y=ypos - 0.05,
        s=str(median),
        verticalalignment="top",
        horizontalalignment="center",
        color=median_line.get_color()
    )
ax = plt.gca()
ax.set_yticks([i for i in range(len(datasets))])
ax.set_yticklabels([ds.label() for ds in datasets])

plt.savefig("plot_exp_openssl_overhead.png", bbox_inches="tight", dpi=300)

