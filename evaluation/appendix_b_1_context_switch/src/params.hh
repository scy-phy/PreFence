#pragma once

#define EXPERIMENT_CPU 3
#define NO_SAMPLES (10000)

#define SHM_NAME ("/context-measure")
#define SHM_SIZE (1024)

#define INTEL_CLOCK_RDTSCP 0
#define INTEL_CLOCK_MONOTONIC 1
#define INTEL_CLOCK_SOURCE INTEL_CLOCK_RDTSCP