#!/usr/bin/env python3

import re
import json

import matplotlib.pyplot as plt

def plot(system, system_short):
	filepaths = [
		("Stock kernel, prefetching disabled (baseline)", "#ff7f0e", [
			(1, "httpit-kern_stock-pf_disabled-1-0.log"),
			(2, "httpit-kern_stock-pf_disabled-2-0.log"),
			(4, "httpit-kern_stock-pf_disabled-4-0.log"),
			(8, "httpit-kern_stock-pf_disabled-8-0.log"),
			(16, "httpit-kern_stock-pf_disabled-16-0.log"),
			(32, "httpit-kern_stock-pf_disabled-32-0.log"),
			(64, "httpit-kern_stock-pf_disabled-64-0.log"),
			(128, "httpit-kern_stock-pf_disabled-128-0.log"),
			(256, "httpit-kern_stock-pf_disabled-256-0.log"),
			(512, "httpit-kern_stock-pf_disabled-512-0.log"),
			(1024, "httpit-kern_stock-pf_disabled-1024-0.log"),
			(2048, "httpit-kern_stock-pf_disabled-2048-0.log"),
			(4096, "httpit-kern_stock-pf_disabled-4096-0.log"),
			(8192, "httpit-kern_stock-pf_disabled-8192-0.log"),
			(16384, "httpit-kern_stock-pf_disabled-16384-0.log"),
			(32768, "httpit-kern_stock-pf_disabled-32768-0.log"),
			(65536, "httpit-kern_stock-pf_disabled-65536-0.log"),
			(131072, "httpit-kern_stock-pf_disabled-131072-0.log"),
			(262144, "httpit-kern_stock-pf_disabled-262144-0.log"),
			(524288, "httpit-kern_stock-pf_disabled-524288-0.log"),
			(1048576, "httpit-kern_stock-pf_disabled-1048576-0.log"),
		]),
		("Stock kernel, prefetching enabled", "#1f77b4", [
			(1, "httpit-kern_stock-pf_enabled-1-0.log"),
			(2, "httpit-kern_stock-pf_enabled-2-0.log"),
			(4, "httpit-kern_stock-pf_enabled-4-0.log"),
			(8, "httpit-kern_stock-pf_enabled-8-0.log"),
			(16, "httpit-kern_stock-pf_enabled-16-0.log"),
			(32, "httpit-kern_stock-pf_enabled-32-0.log"),
			(64, "httpit-kern_stock-pf_enabled-64-0.log"),
			(128, "httpit-kern_stock-pf_enabled-128-0.log"),
			(256, "httpit-kern_stock-pf_enabled-256-0.log"),
			(512, "httpit-kern_stock-pf_enabled-512-0.log"),
			(1024, "httpit-kern_stock-pf_enabled-1024-0.log"),
			(2048, "httpit-kern_stock-pf_enabled-2048-0.log"),
			(4096, "httpit-kern_stock-pf_enabled-4096-0.log"),
			(8192, "httpit-kern_stock-pf_enabled-8192-0.log"),
			(16384, "httpit-kern_stock-pf_enabled-16384-0.log"),
			(32768, "httpit-kern_stock-pf_enabled-32768-0.log"),
			(65536, "httpit-kern_stock-pf_enabled-65536-0.log"),
			(131072, "httpit-kern_stock-pf_enabled-131072-0.log"),
			(262144, "httpit-kern_stock-pf_enabled-262144-0.log"),
			(524288, "httpit-kern_stock-pf_enabled-524288-0.log"),
			(1048576, "httpit-kern_stock-pf_enabled-1048576-0.log"),
		]),
		("Patched kernel, prefetching enabled", "#2ca02c", [
			(1, "httpit-kern_patched-pf_enabled-1-0.log"),
			(2, "httpit-kern_patched-pf_enabled-2-0.log"),
			(4, "httpit-kern_patched-pf_enabled-4-0.log"),
			(8, "httpit-kern_patched-pf_enabled-8-0.log"),
			(16, "httpit-kern_patched-pf_enabled-16-0.log"),
			(32, "httpit-kern_patched-pf_enabled-32-0.log"),
			(64, "httpit-kern_patched-pf_enabled-64-0.log"),
			(128, "httpit-kern_patched-pf_enabled-128-0.log"),
			(256, "httpit-kern_patched-pf_enabled-256-0.log"),
			(512, "httpit-kern_patched-pf_enabled-512-0.log"),
			(1024, "httpit-kern_patched-pf_enabled-1024-0.log"),
			(2048, "httpit-kern_patched-pf_enabled-2048-0.log"),
			(4096, "httpit-kern_patched-pf_enabled-4096-0.log"),
			(8192, "httpit-kern_patched-pf_enabled-8192-0.log"),
			(16384, "httpit-kern_patched-pf_enabled-16384-0.log"),
			(32768, "httpit-kern_patched-pf_enabled-32768-0.log"),
			(65536, "httpit-kern_patched-pf_enabled-65536-0.log"),
			(131072, "httpit-kern_patched-pf_enabled-131072-0.log"),
			(262144, "httpit-kern_patched-pf_enabled-262144-0.log"),
			(524288, "httpit-kern_patched-pf_enabled-524288-0.log"),
			(1048576, "httpit-kern_patched-pf_enabled-1048576-0.log"),
		]),
		("Patched kernel, PreFence (plugin level)", "#9467bd", [
			(1, "httpit-kern_patched-prefence_plugin-1-0.log"),
			(2, "httpit-kern_patched-prefence_plugin-2-0.log"),
			(4, "httpit-kern_patched-prefence_plugin-4-0.log"),
			(8, "httpit-kern_patched-prefence_plugin-8-0.log"),
			(16, "httpit-kern_patched-prefence_plugin-16-0.log"),
			(32, "httpit-kern_patched-prefence_plugin-32-0.log"),
			(64, "httpit-kern_patched-prefence_plugin-64-0.log"),
			(128, "httpit-kern_patched-prefence_plugin-128-0.log"),
			(256, "httpit-kern_patched-prefence_plugin-256-0.log"),
			(512, "httpit-kern_patched-prefence_plugin-512-0.log"),
			(1024, "httpit-kern_patched-prefence_plugin-1024-0.log"),
			(2048, "httpit-kern_patched-prefence_plugin-2048-0.log"),
			(4096, "httpit-kern_patched-prefence_plugin-4096-0.log"),
			(8192, "httpit-kern_patched-prefence_plugin-8192-0.log"),
			(16384, "httpit-kern_patched-prefence_plugin-16384-0.log"),
			(32768, "httpit-kern_patched-prefence_plugin-32768-0.log"),
			(65536, "httpit-kern_patched-prefence_plugin-65536-0.log"),
			(131072, "httpit-kern_patched-prefence_plugin-131072-0.log"),
			(262144, "httpit-kern_patched-prefence_plugin-262144-0.log"),
			(524288, "httpit-kern_patched-prefence_plugin-524288-0.log"),
			(1048576, "httpit-kern_patched-prefence_plugin-1048576-0.log"),
		]),
		("Patched kernel, PreFence (applic. level)", "#e377c2", [
			(1, "httpit-kern_patched-prefence_app-1-0.log"),
			(2, "httpit-kern_patched-prefence_app-2-0.log"),
			(4, "httpit-kern_patched-prefence_app-4-0.log"),
			(8, "httpit-kern_patched-prefence_app-8-0.log"),
			(16, "httpit-kern_patched-prefence_app-16-0.log"),
			(32, "httpit-kern_patched-prefence_app-32-0.log"),
			(64, "httpit-kern_patched-prefence_app-64-0.log"),
			(128, "httpit-kern_patched-prefence_app-128-0.log"),
			(256, "httpit-kern_patched-prefence_app-256-0.log"),
			(512, "httpit-kern_patched-prefence_app-512-0.log"),
			(1024, "httpit-kern_patched-prefence_app-1024-0.log"),
			(2048, "httpit-kern_patched-prefence_app-2048-0.log"),
			(4096, "httpit-kern_patched-prefence_app-4096-0.log"),
			(8192, "httpit-kern_patched-prefence_app-8192-0.log"),
			(16384, "httpit-kern_patched-prefence_app-16384-0.log"),
			(32768, "httpit-kern_patched-prefence_app-32768-0.log"),
			(65536, "httpit-kern_patched-prefence_app-65536-0.log"),
			(131072, "httpit-kern_patched-prefence_app-131072-0.log"),
			(262144, "httpit-kern_patched-prefence_app-262144-0.log"),
			(524288, "httpit-kern_patched-prefence_app-524288-0.log"),
			(1048576, "httpit-kern_patched-prefence_app-1048576-0.log"),
		]),
	]

	# Read a wrk logfile, extract the number of requests per seconds
	def read_file(filepath: str) -> float:
		with open(filepath) as file:
			file_json = json.loads(file.read())
			return file_json["rpsAvg"]

	xvals = []
	yvals = {}

	for group_label, group_color, group_yvals_and_paths in filepaths:
		yvals[(group_label, group_color)] = []
		for idx, (xval, filepath) in enumerate(group_yvals_and_paths):
			req_per_s = read_file(filepath)
			yvals[(group_label, group_color)].append(req_per_s)
			if len(xvals) <= idx:
				xvals.append(xval)
			else:
				assert xval == xvals[idx]

	yvals_reference = ('Stock kernel, prefetching disabled (baseline)', '#ff7f0e')
	for key, trace in yvals.items():
		if key != yvals_reference:
			yvals[key] = [
				value / reference for value, reference in zip(trace, yvals[yvals_reference])
			]
	yvals[yvals_reference] = [
		value / reference for value, reference in zip(yvals[yvals_reference], yvals[yvals_reference])
	]
	
	plt.clf()
	fig = plt.gcf()
	fig.set_size_inches(6, 3)

	plt.rcParams["font.family"] = "Nimbus Roman"
	plt.rcParams["font.size"] = 12
	ax = plt.gca()
	ax.set_xscale("log", base=2)
	ax.set_xticks(xvals)
	ax.set_ylim(0.98, 1.32)
	markers = ["+", "^", "v", ">", "x"]
	for ((line_label, line_color), line_yvals), marker in zip(yvals.items(), markers):
		plt.plot(
			xvals, line_yvals,
			label=line_label,
			marker=marker,
			color=line_color,
			alpha=0.5,
		)
	plt.xlabel("number of rows (log scale)")
	plt.ylabel("relative performance")
	plt.title(system)
	plt.legend(loc=2, prop={'size': 9.5})
	plt.savefig(f"plot_lighttpd_progressive_{system_short}.svg", bbox_inches="tight")

# plot(
# 	system="Raspberry Pi 4 (Broadcom BCM2711, Cortex-A72)",
# 	system_short="rpi4"
# )

# plot(
# 	system="Intel Core i7-10510U",
# 	system_short="i7-10510U"
# )
