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

logfiles = [ # file path, label
    "results_1M_countermeasure_mixed_0.log",
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
            rx_logline = re.match(r"^L;(?P<use_cm_flag>[01]);(?P<misses>\d+);(?P<time_compute>\d+);(?P<time_reload>\d+)$", line)
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

plt.rcParams["font.family"] = "Nimbus Roman"

for ds in datasets:
    plt.clf()
    ax = plt.gca()
    ax.set_xlim(0, 20)
    ax.set_xticks(range(0, 21))
    ax.set_xticklabels(range(0, 21))
    ax.set_yscale('log')
    plt.hist(ds.misses, bins=21, range=(0, 21), align="left", edgecolor="black")
    plt.xlabel("number of misses")
    plt.ylabel("count")
    plt.title(f"OpenSSL / pf_disable flag {ds.label()}")
    plt.savefig(f"plot_misses_{ds.label()}.png")
