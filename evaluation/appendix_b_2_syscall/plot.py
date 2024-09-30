import re

import matplotlib.pyplot as plt

logfile = "out.log"
durations = {
    "set": [],
    "clear": [],
}

with open(logfile) as file:
    for line in file:
        rx = re.match(r"^(?P<duration_set>\d+) (?P<duration_clear>\d+)$", line)
        if rx:
            durations["set"].append(int(rx.group("duration_set")))
            durations["clear"].append(int(rx.group("duration_clear")))
        else:
            print("Skipped", line.strip())

assert len(durations["set"]) == len(durations["clear"])

fig = plt.gcf()
fig.set_size_inches(5, 4)

plt.rcParams["font.family"] = "Nimbus Roman"
plt.rcParams["font.size"] = 12

plt.subplot(2, 1, 1)

# plt.xlabel("time (rdtscp units)")
plt.ylabel("number of samples")

# histogram figure
ax = plt.gca()
# ax.set_xlim(280, 800)
for trace_label, trace_data in durations.items():
    plt.hist(
        x=trace_data,
        label=trace_label,
        bins=200,
        # range=(280, 800),
        alpha=0.6,
    )
plt.legend()

# boxplot figure
plt.subplot(2, 1, 2)
ax = plt.gca()
# ax.set_xlim(280, 800)
plt.xlabel("time (rdtscp units)")
plt.ylabel("prefetch_disable")
for ypos, (trace_label, trace_data) in enumerate(reversed(durations.items())):
    boxplot = plt.boxplot(
        x=trace_data,
        positions=[ypos],
        vert=False,
        widths=0.5
    )
    for median_line in boxplot["medians"]:
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
ax.set_yticks([i for i in range(len(durations))])
ax.set_yticklabels([key for key in reversed(durations.keys())])
plt.savefig("plot_both.svg", bbox_inches="tight")
