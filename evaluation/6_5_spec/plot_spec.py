#!/usr/bin/env python3

import re
import json
from decimal import Decimal

import matplotlib
import matplotlib.pyplot as plt
from matplotlib.patches import Patch

### DO NOT RUN THIS FILE DIRECTLY.
### CALL THE PLOT FUNCTION INSTEAD.

def plot(system, system_short, benchmark_suite, csvfilepaths, include_legend):
	# Parse SPEC CSV file
	data = {}
	colors = {}
	indicator_table_start = "\"Full Results Table\""
	indicator_table_end = "\"Selected Results Table\""
	indicator_score_line = "SPECrate2017_int_base"
	for csvfile_label, csvfile_color, csvfile_path in csvfilepaths:
		with open(csvfile_path) as csvfile:
			# collect benchmark result table lines and final score line
			result_lines = []
			score_line = ""
			collecting = False
			for line in csvfile.readlines():
				# print(line, end="")
				if collecting == False and line.startswith(indicator_table_start):
					collecting = True
				elif collecting == True and line.startswith(indicator_table_end):
					collecting = False
				elif collecting == True and line != "\n":
					result_lines.append(line)
				elif collecting == False and line.startswith(indicator_score_line):
					score_line = line
					break

			# parse benchmark result table
			headers = result_lines[0].split(",")
			for i, _ in enumerate(headers):
				headers[i] = headers[i].strip("\"\n")
			benchmarks = []
			result_lines = result_lines[1:]
			for line in result_lines:
				fields = line.split(",")
				benchmark = {}
				assert len(fields) == len(headers)
				for idx, field in enumerate(fields):
					header = headers[idx]
					field = field.strip("\"\n")
					if header.startswith("Peak"):
						continue
					elif header in ["Base Run Time", "Base Rate"]:
						benchmark[header] = Decimal(field)
					elif header in ["Base Selected"]:
						if field in ["0", "1"]:
							benchmark[header] = True if field == "1" else False
						else:
							raise Exception("Unexpected value in boolean field")
					else:
						benchmark[header] = field
				benchmarks.append(benchmark)

			# parse final score line
			rx_base_score = re.match(r"^SPECrate2017_int_base,(?P<base_score>[\d.]+),.*", score_line)
			if rx_base_score:
				base_score_decimal = Decimal(rx_base_score.group("base_score"))
			else:
				raise Exception("Invalid base_score")
			
			# store data in python dict
			data[csvfile_label] = {
				"base_score": base_score_decimal,
				"benchmarks": benchmarks,
			}	
			colors[csvfile_label] = csvfile_color

	# group data per benchmark
	benchmark_data_agg = {}
	for csvfile_label, csvfile_data in data.items():
		for benchmark_data in csvfile_data["benchmarks"]:
			benchmark_name = benchmark_data["Benchmark"].replace(".", ".\n")
			benchmark_time = benchmark_data["Base Run Time"]
			benchmark_rate = benchmark_data["Base Rate"]
			benchmark_selected = benchmark_data["Base Selected"]
			if benchmark_name not in benchmark_data_agg:
				benchmark_data_agg[benchmark_name] = {}
			if csvfile_label not in benchmark_data_agg[benchmark_name]:
				benchmark_data_agg[benchmark_name][csvfile_label] = {
					"time": [],
					"rate": [],
				}
			benchmark_data_agg[benchmark_name][csvfile_label]["time"].append(float(benchmark_time))
			benchmark_data_agg[benchmark_name][csvfile_label]["rate"].append(float(benchmark_rate))
			benchmark_data_agg[benchmark_name][csvfile_label]["time"].sort()
			benchmark_data_agg[benchmark_name][csvfile_label]["rate"].sort()

	print(json.dumps(benchmark_data_agg, indent=2))

	# extract data for plotting
	ylabels = []
	yticks = []
	yvals = []
	csvfile_labels = []
	xvals_time = []
	xvals_rate = []
	xerrs_time_min = []
	xerrs_time_max = []
	xerrs_rate_min = []
	xerrs_rate_max = []
	yval_current = -1

	for benchmark_name, benchmark_data in benchmark_data_agg.items():
		for idx, (csvfile_label, values) in enumerate(benchmark_data.items()):
			if len(csvfile_labels) < len(benchmark_data):
				csvfile_labels.append(csvfile_label)
			# ylabels.append(f"{benchmark_name}/{csvfile_label}")
			# yticks.append(yval_current)
			if idx == len(benchmark_data) // 2:
				ylabels.append(benchmark_name)
				yticks.append(yval_current)
			yvals.append(yval_current)
			yval_current -= 10
			assert len(values["time"]) == 3
			assert len(values["rate"]) == 3
			xerrs_time_min.append(values["time"][1] - values["time"][0])
			xvals_time.append(values["time"][1])
			xerrs_time_max.append(values["time"][2] - values["time"][1])
			xerrs_rate_min.append(values["rate"][1] - values["rate"][0])
			xvals_rate.append(values["rate"][1])
			xerrs_rate_max.append(values["rate"][2] - values["rate"][1])
		yval_current -= 2

	print(csvfile_labels)

	csvfile_colors = [colors[label] for label in csvfile_labels]

	# plot data per benchmark
	matplotlib.use("qtagg")
	fig = plt.gcf()
	fig.set_size_inches(3.2, 5.2)

	plt.rcParams["font.family"] = "Nimbus Roman"
	plt.rcParams["font.size"] = 12

	plt.barh(
		y=yvals,
		width=xvals_time,
		height=8,
		xerr=[xerrs_time_min, xerrs_time_max],
		color=csvfile_colors,
	)
	for xval, yval in zip(xvals_time, yvals):
		plt.text(
			s=f"  {round(xval)}",
			x=xval, y=yval,
			horizontalalignment="left",
			verticalalignment="center",
			size=10,
			# weight="bold",
		)
	ax = plt.gca()
	ax.set_yticks(yticks)
	# ax.set_yticklabels([ylabel.replace(".", ".\n") for ylabel in ylabels])
	ax.set_yticklabels(ylabels)
	ax.set_xlabel("median time in seconds (lower is better)")
	ax.set_ylabel("benchmark")
	ax.set_ylim([yvals[len(yvals)-1]-11, yvals[0]+11])
	ax.set_xlim([0, 2000])
	plt.title(f"{benchmark_suite}: {system}")

	if include_legend == True:
		legend_patches = [Patch(facecolor=color) for color in csvfile_colors]
		ax.legend(
			handles=legend_patches,
			labels=csvfile_labels,
		)

	plt.savefig(f"plot_spec_time_{system_short}.svg", bbox_inches="tight")

	# plt.clf()

	# plt.barh(
	# 	y=yvals,
	# 	width=xvals_rate,
	# 	xerr=[xerrs_rate_min, xerrs_rate_max],
	# 	color=["#1f77b4", "#ff7f0e", "#2ca02c"],
	# )
	# ax = plt.gca()
	# ax.set_yticks(yvals)
	# ax.set_yticklabels(ylabels)
	# ax.set_xlabel("median rate; higher is better; iterations = 3; error bars show minimum and maximum value")
	# ax.set_ylabel("benchmark/configuration")
	# ax.set_ylim([yvals[len(yvals)-1]-1.1, yvals[0]+1.1])
	# plt.title(f"{benchmark}: {system}: Rate")
	# plt.savefig("plot_rate.svg", bbox_inches="tight")

if __name__ == "__main__":
	raise Exception("Do not run this file directly. Import it and call the plot function instead.")
