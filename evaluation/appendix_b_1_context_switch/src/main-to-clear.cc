#include <cstdlib>
#include <cstdio>

#include <sched.h>
#include <signal.h>

#include "params.hh"
#include "utils.hh"

shm_data_t shm_data = {-1, 0};

void sigint_handler(int signum) {
	if (signum == SIGINT) {
		fprintf(stderr, "Interrupted.\n");
	}
	fflush(stderr);
	unmap_shm(shm_data);
	exit(EXIT_SUCCESS);
}

int main() {
	if (pin_process_to_cpu(0, EXPERIMENT_CPU) == -1) {
		perror("pin_process_to_cpu");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "CPU: %d\n", sched_getcpu());
	fprintf(stderr, "Collecting %d samples\n", NO_SAMPLES);

	signal(SIGINT, &sigint_handler);
	shm_data =  map_shm(false);
	
	volatile uint64_t* value = (uint64_t*)shm_data.ptr;
	uint64_t prev_counter = 0;
	for (size_t samples = 0; samples < NO_SAMPLES;) {
		uint64_t current = *value;
		uint64_t now = rdtsc();
		uint64_t counter = current >> 48;
		uint64_t delta = now - (current & ((1ULL << 48)-1));
		if (counter != prev_counter) {
			printf("%lu %lu\n", counter, delta);
			prev_counter = counter;
			samples++;
		}
		sched_yield();
	}
	fprintf(stderr, "Done.\n");
	sigint_handler(-1);
}