#!/usr/bin/env python3

import re
import json
import glob
from enum import Enum
import matplotlib.pyplot as plt

class FigureType(Enum):
	ABSOLUTE_RUNTIME = 1
	ABSOLUTE_RUNTIME_DIFFERENCE = 2
	RELATIVE_RUNTIME_DIFFERENCE = 3
	RELATIVE_PERFORMANCE_DIFFERENCE = 4

###########################################################################

# DATASET = "log2"
# DATASET = "log10"
DATASET = "16M"

COLORS = {
	"Stock kernel, PF'ing disabled (baseline)": "#000000",
	"Stock kernel, PF'ing enabled": "#1f77b4",
	"Patched kernel, PF'ing enabled": "#2ca02c",
	"Patched kernel, PreFence (fine-grained)": "#9467bd",
	"Patched kernel, PreFence (coarse-grained)": "#ff7f0e",
}
MARKERS = {
	"Stock kernel, PF'ing disabled (baseline)": "+",
	"Stock kernel, PF'ing enabled": "^",
	"Patched kernel, PF'ing enabled": "v",
	"Patched kernel, PreFence (fine-grained)": ">",
	"Patched kernel, PreFence (coarse-grained)": "x",
}
REFERENCE_LABEL = "Stock kernel, PF'ing disabled (baseline)"

# This script can plot three different figure types. Select the desired
# figure type here.
# - ABSOLUTE_RUNTIME: y-axis depicts the absolute time required for the
#   file upload.
# - ABSOLUTE_RUNTIME_DIFFERENCE: y-axis depicts the absolute time
#   difference (in seconds) between the current series and the respective
#   median value of a reference series (specified by REFERENCE_LABEL above)
#   at the same x-position.
# - RELATIVE_RUNTIME_DIFFERENCE: Same as ABSOLUTE_RUNTIME_DIFFERENCE, but
#   the difference to the reference series is specified in relative terms
#   (i.e., as a ratio) instead of absolute values.
# - RELATIVE_PERFORMANCE_DIFFERENCE: 1/RELATIVE_RUNTIME_DIFFERENCE

# FIGURE_TYPE = FigureType.ABSOLUTE_RUNTIME
# FIGURE_TYPE = FigureType.ABSOLUTE_RUNTIME_DIFFERENCE
# FIGURE_TYPE = FigureType.RELATIVE_RUNTIME_DIFFERENCE
FIGURE_TYPE = FigureType.RELATIVE_PERFORMANCE_DIFFERENCE

###########################################################################

def plot(system: str, system_short: str) -> None:
	# Labels for data series and paths to the respective json files to plot
	series_meta = [
		(
			"Stock kernel, PF'ing disabled (baseline)",
			glob.glob(f"{system_short}/results_kern_stock-pf_disabled-*.json")
		),
		(
			"Patched kernel, PreFence (coarse-grained)",
			glob.glob(f"{system_short}/results_kern_patched-prefence_app-*.json")
		),
		(
			"Patched kernel, PreFence (fine-grained)",
			glob.glob(f"{system_short}/results_kern_patched-prefence_plugin-*.json")
		),
		(
			"Patched kernel, PF'ing enabled",
			glob.glob(f"{system_short}/results_kern_patched-pf_enabled-*.json")
		),
		(
			"Stock kernel, PF'ing enabled",
			glob.glob(f"{system_short}/results_kern_stock-pf_enabled-*.json")
		),
	]

	def get_xval_from_upload_file_path(upload_file_path: str) -> int:
		rx = re.match(r".*(log10|log2|16M)_(?P<xval>\d+)\.bin$", upload_file_path)
		return int(rx.group("xval"))

	# Read JSON logfiles for a series, extract upload time
	def read_json_logfiles(filepath_glob: str) -> list[tuple[int, float]]:
		datapoints = []
		with open(filepath) as file:
			file_json = json.loads(file.read())
			for datapoint in file_json:
				if DATASET in datapoint["upload_file_path"]:
					datapoints.append(
						(
							get_xval_from_upload_file_path(datapoint["upload_file_path"]),
							datapoint["time_upload_s"]
						)
					)
		datapoints.sort(key=lambda datapoint: datapoint[0])
		return datapoints

	# Data preparation
	series = {}
	# for each series...
	for series_label, series_paths in series_meta:
		# read the respective json files (one per repetition of the
		# experiment) and extract data points as tuples (x, y); x = file
		# size, y = upload time.
		# Result: Points = [(xval1, yval1), ..., (xvalM, yvalN)]
		points = []
		for filepath in series_paths:
			points_file = read_json_logfiles(filepath)
			points.extend(points_file)

		# aggregate all the measurements (y) that we made for a particular
		# file size (x) in a dict.
		# Result: buckets_by_xval[xval] = [yval1, yval2, ..., yvalN]
		buckets_by_xval = {}
		for point_x, point_y in points:
			if point_x not in buckets_by_xval:
				buckets_by_xval[point_x] = []
			buckets_by_xval[point_x].append(point_y)

		# store result in series data structure for later plotting
		series[series_label] = buckets_by_xval

	# We can either plot the absolute runtime for each series, or express
	# runtimes relatively to one of the series. If we opt for a relative
	# representation, we need to compute the ratios here and update the
	# series dict accordingly.
	if FIGURE_TYPE != FigureType.ABSOLUTE_RUNTIME:
		# compute medians of the reference series
		reference_medians = {
			xval: list(sorted(yvals))[len(yvals)//2]
			for xval, yvals in series[REFERENCE_LABEL].items()
		}
		# compute ratios
		for label, buckets_by_xval in series.items():
			for xval, yvals in buckets_by_xval.items():
				new_yvals = []
				for yval in yvals:
					if FIGURE_TYPE == FigureType.RELATIVE_RUNTIME_DIFFERENCE:
						new_yvals.append(yval / reference_medians[xval])
					elif FIGURE_TYPE == FigureType.RELATIVE_PERFORMANCE_DIFFERENCE:
						new_yvals.append(1 / (yval / reference_medians[xval]))
					elif FIGURE_TYPE == FigureType.ABSOLUTE_RUNTIME_DIFFERENCE:
						new_yvals.append(yval - reference_medians[xval])
					else:
						assert False
				buckets_by_xval[xval] = new_yvals

	# Start plotting

	# General setup
	plt.clf()
	fig = plt.gcf()
	if DATASET == "log2":
		fig.set_size_inches(12, 3)
	else:
		fig.set_size_inches(6, 3)
	plt.rcParams["font.family"] = "Nimbus Roman"
	plt.rcParams["font.size"] = 12
	ax = plt.gca()
	
	# for each series...
	for series_label, series_buckets_by_xval in series.items():
		color = COLORS[series_label]
		marker = MARKERS[series_label]

		# rearrange data for matplotlib's boxplot function
		xvals = []
		yval_buckets = []
		for xval, yval_bucket in series_buckets_by_xval.items():
			xvals.append(xval)
			yval_buckets.append(yval_bucket)

		# plot the boxplots

		if DATASET == "16M":
			boxplot_widths = 6 * 1024 * 1024
		elif DATASET in ["log2", "log10"]:
			boxplot_widths = [0.5 * xval for xval in xvals]
		else:
			assert False

		boxplot_data = plt.boxplot(
			yval_buckets,
			positions=xvals,
			vert=True,
			showfliers=False,
			# whis=(0, 100),
			whis=(2, 98), # drop top/bottom 2%
			widths=boxplot_widths, # 6 * 1024 * 1024,
			
			patch_artist=True, # fill boxes with boxprops.facecolor
			boxprops=dict(facecolor=color, color=color, alpha=0.5),
			capprops=dict(color=color, alpha=1),
			whiskerprops=dict(color=color, alpha=0.5),
			flierprops=dict(color=color, markeredgecolor=color, alpha=0.5),
			medianprops=dict(color=color, alpha=1),
		)

		# plot a line connecting the medians of the series
		medians = [median.get_ydata()[0] for median in boxplot_data['medians']]
		plt.plot(
			xvals,
			medians,
			color=color,
			# marker=marker,
			label=series_label,
			alpha=1,
			zorder=2,
		)
		# print the medians of the series
		print(series_label)
		print(f" - Medians: {[f'(x={x},med={m:.3f})' for x, m in zip(xvals, medians)]}")

	# plot grid
	for xval in xvals:
		plt.axvline(
			xval,
			color="#ccc",
			alpha=0.3,
			linestyle="dotted",
			zorder=-1,
		)

	# Set up axes and labels
	if DATASET == "log2":
		ax.set_xscale("log", base=2)
	elif DATASET == "log10":
		ax.set_xscale("log", base=10)
	ax.set_xticks(xvals)
	if DATASET == "16M":
		ax.set_xticklabels([f"{xval / 1024**2:.0f}" for xval in xvals])
		plt.xlabel("file size (MiB)")
	else:
		plt.xlabel("file size (B)")

	if FIGURE_TYPE == FigureType.ABSOLUTE_RUNTIME:
		ax.set_ylim(0, 0.5)
		plt.ylabel("duration (s)")
	elif FIGURE_TYPE == FigureType.ABSOLUTE_RUNTIME_DIFFERENCE:
		ax.set_ylim(-0.08, 0.06)
		plt.ylabel("absolute duration difference (s)")
	elif FIGURE_TYPE == FigureType.RELATIVE_RUNTIME_DIFFERENCE:
		ax.set_ylim(0.78, 1.17)
		plt.ylabel("relative duration")
	elif FIGURE_TYPE == FigureType.RELATIVE_PERFORMANCE_DIFFERENCE:
		ax.set_ylim(0.85, 1.28)
		plt.ylabel("relative performance")

	plt.title(system)
	# plt.legend(
	# 	# loc=2,
	# 	prop={'size': 9.5},
	# 	ncol=2,
	# )
	
	# Save plot to file
	plt_filename = f"plot_lighttpd_upload_{system_short}_{DATASET}_{str(FIGURE_TYPE.name).lower()}.svg"
	print(plt_filename, "\n")
	plt.savefig(plt_filename, bbox_inches="tight")


plot(
	system="Raspberry Pi 4 (Broadcom BCM2711, Cortex-A72)",
	system_short="rpi4-uploadonly"
)

plot(
	system="Intel Core i7-10510U",
	system_short="i7-10510U-uploadonly",
)


