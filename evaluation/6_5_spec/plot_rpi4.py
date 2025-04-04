#!/usr/bin/env python3

from plot_spec import plot

plot(
	system="Raspberry Pi 4 (Broadcom BCM2711, Cortex-A72)",
	system_short="rpi4",
	benchmark_suite="SPECrate2017_int_base",
	csvfilepaths=[ # label, path
		("Stock Kernel,\nPF'ing disabled", "#ff7f0e", "result-rpi4-pfendis/CPU2017.001.intrate.csv"),
		("Stock Kernel,\nPF'ing enabled", "#1f77b4", "result-rpi4-pfen/CPU2017.001.intrate.csv"),
		("Patched Kernel,\nPF'ing enabled", "#2ca02c", "result-rpi4-pfdis/CPU2017.002.intrate.csv"),
	],
	include_legend=False,
)