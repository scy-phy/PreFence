#include <cstdlib>
#include <cstdio>

#include <sys/prctl.h>
#define PR_SET_PREFETCH_DISABLE         128
#define PR_GET_PREFETCH_DISABLE         129

#include "params.hh"
#include "utils.hh"

int main() {
	if (pin_process_to_cpu(0, EXPERIMENT_CPU) == -1) {
		perror("pin_process_to_cpu");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "CPU: %d\n", sched_getcpu());
	fprintf(stderr, "Collecting %d samples\n", NO_SAMPLES);

	for (size_t samples = 0; samples < NO_SAMPLES; samples++) {
		uint64_t before_set, after_set;
		// ###############################################
		{
			before_set = rdtsc();
			int ret = prctl(PR_SET_PREFETCH_DISABLE, 1, NULL, NULL);
			after_set = rdtsc();
			if (ret < 0) { perror("prctl failed"); exit(EXIT_FAILURE); }
			if (ret != 0) { printf("prctrl returned unexpected value %d\n", ret); exit(EXIT_FAILURE); }
		}
		// ###############################################

		uint64_t before_clear, after_clear;
		// ###############################################
		{
			before_clear = rdtsc();
			int ret = prctl(PR_SET_PREFETCH_DISABLE, 1, NULL, NULL);
			after_clear = rdtsc();
			if (ret < 0) { perror("prctl failed"); exit(EXIT_FAILURE); }
			if (ret != 0) { printf("prctrl returned unexpected value %d\n", ret); exit(EXIT_FAILURE); }
		}
		// ###############################################
		printf(
			"%lu %lu\n",
			after_set - before_set,
			after_clear - before_clear
		);
	}
	fprintf(stderr, "Done.\n");
	return 0;
}