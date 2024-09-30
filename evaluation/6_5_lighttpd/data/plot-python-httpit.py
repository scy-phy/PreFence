#!/usr/bin/env python3

import re
import json
from decimal import Decimal

import matplotlib.pyplot as plt

def plot(system, system_short):
	filepaths = [
		("Stock kernel,\nprefetching disabled", "#ff7f0e", [
			"httpit-kern_stock-pf_disabled-0.log",
			"httpit-kern_stock-pf_disabled-1.log",
			"httpit-kern_stock-pf_disabled-2.log",
		]),
		("Stock kernel,\nprefetching enabled", "#1f77b4", [
			"httpit-kern_stock-pf_enabled-0.log",
			"httpit-kern_stock-pf_enabled-1.log",
			"httpit-kern_stock-pf_enabled-2.log",
		]),
		("Patched kernel\nprefetching enabled", "#2ca02c", [
			"httpit-kern_patched-pf_enabled-0.log",
			"httpit-kern_patched-pf_enabled-1.log",
			"httpit-kern_patched-pf_enabled-2.log",
		]),
		("Patched kernel\nPreFence (plugin level)", "#9467bd", [
			"httpit-kern_patched-prefence_plugin-0.log",
			"httpit-kern_patched-prefence_plugin-1.log",
			"httpit-kern_patched-prefence_plugin-2.log",
		]),
		("Patched kernel,\nPreFence (applic. level)", "#e377c2", [
			"httpit-kern_patched-prefence_app-0.log",
			"httpit-kern_patched-prefence_app-1.log",
			"httpit-kern_patched-prefence_app-2.log",
		]),
	]

	# Read a wrk logfile, extract the number of requests per seconds
	def read_file(filepath: str) -> float:
		req_per_s_decimal = Decimal(-1)
		with open(filepath) as file:
			file_json = json.loads(file.read())
		return file_json["rpsAvg"]

	ylabels = []
	xvals = []
	xerrs_min = []
	xerrs_max = []
	colors = []
	for group_label, group_color, group_paths in filepaths:
		results_req_per_s = []
		for filepath in group_paths:
			req_per_s = read_file(filepath)
			results_req_per_s.append(req_per_s)
		results_req_per_s.sort()
		print(group_label.replace('\n', ' '), results_req_per_s)
		median = results_req_per_s[len(results_req_per_s) // 2]
		ylabels.append(group_label)
		colors.append(group_color)
		xvals.append(median)
		xerrs_min.append(median - results_req_per_s[0])
		xerrs_max.append(results_req_per_s[-1] - median)
	yvals = [-i for i in range(len(ylabels))]

	plt.clf()
	fig = plt.gcf()
	fig.set_size_inches(4, 2.5)

	plt.rcParams["font.family"] = "Nimbus Roman"
	plt.rcParams["font.size"] = 12

	ax = plt.gca()
	ax.set_yticks(yvals)
	ax.set_yticklabels(ylabels)
	ax.set_xlabel("requests per second (more is better)")
	# ax.set_ylabel("Configuration")

	plt.title(system)
	plt.barh(
		y=yvals,
		width=xvals,
		height=0.7,
		xerr=[xerrs_min, xerrs_max],
		color=colors,
	)

	for xval, yval in zip(xvals, yvals):
		plt.text(
			s=f"{xval:.2f}  ",
			x=xval, y=yval,
			horizontalalignment="right",
			verticalalignment="center",
			color="white",
			weight="bold",
		)

	plt.savefig(f"plot_lighttpd_{system_short}.svg", bbox_inches="tight")

# plot(
# 	system="Raspberry Pi 4 (Broadcom BCM2711, Cortex-A72)",
# 	system_short="rpi4"
# )

# plot(
# 	system="Intel Core i7-10510U",
# 	system_short="i7-10510U"
# )

