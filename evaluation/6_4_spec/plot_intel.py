#!/usr/bin/env python3

from plot_spec import plot

plot(
	system="Intel Core i7-10510U",
	system_short="i7-10510U",
	benchmark_suite="SPECrate2017_int_base",
	csvfilepaths=[ # label, path
		("Stock Kernel,\nPF'ing disabled", "#ff7f0e", "result-intel-pfendis/CPU2017.001.intrate.csv"),
		("Stock Kernel,\nPF'ing enabled", "#1f77b4", "result-intel-pfen/CPU2017.001.intrate.csv"),
		("Patched Kernel,\nPF'ing enabled", "#2ca02c", "result-intel-pfdis/CPU2017.001.intrate.csv"),
	],
	include_legend=True,
)