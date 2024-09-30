import re

import matplotlib.pyplot as plt

logfiles = [
    ("out_kernel_stock_clear.log", "stock"),
    ("out_kernel_pf_clear.log", "patched/\nclear→clear"),
    ("out_kernel_pf_set.log", "patched/\nclear→set"),
]
traces = {}

for logfile, label in logfiles:
    with open(logfile) as file:
        timestamps = []
        for line in file:
            rx = re.match(r"^(?P<counter>\d+) (?P<timer>\d+)$", line)
            if rx:
                timestamps.append(int(rx.group("timer")))
            else:
                print("Skipped", line.strip())
        traces[label] = timestamps

fig = plt.gcf()
fig.set_size_inches(5, 4)

plt.rcParams["font.family"] = "Nimbus Roman"
plt.rcParams["font.size"] = 12

plt.subplot(2, 1, 1)

# plt.xlabel("time (rdtscp units)")
plt.ylabel("number of samples")

ax = plt.gca()
# ax.set_xlim(3000, 7000)

# histogram figure
for trace_label, trace_data in traces.items():
    plt.hist(
        x=trace_data,
        label=trace_label,
        bins=200,
        # range=(3000, 7000),
        alpha=0.6,
    )
plt.legend()

# boxplot figure
plt.subplot(2, 1, 2)
ax = plt.gca()
# ax.set_xlim(3000, 7000)
ax.set_ylim(-0.65, 2.4)
plt.xlabel("time (rdtscp units)")
for ypos, (trace_label, trace_data) in enumerate(reversed(traces.items())):
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
ax.set_yticks([i for i in range(len(traces))])
ax.set_yticklabels([key for key in reversed(traces.keys())])
plt.savefig("plot_both.svg", bbox_inches="tight")
