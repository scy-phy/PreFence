#include <cstdlib>
#include <cstdio>
#include <signal.h>

#include "params.hh"
#include "utils.hh"

shm_data_t shm_data = {-1, 0};

void sigint_handler(int signum) {
	fprintf(stderr, "Interrupted.\n");
	unmap_shm(shm_data);
	unlink_shm(shm_data);
	exit(EXIT_FAILURE);
}

int main() {
	if (pin_process_to_cpu(0, EXPERIMENT_CPU) == -1) {
		perror("pin_process_to_cpu");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "CPU: %d\n", sched_getcpu());

	signal(SIGINT, &sigint_handler);
	shm_data = map_shm(true);

	volatile uint64_t* shm = (uint64_t*)shm_data.ptr;
	uint64_t counter = 0;
	while (1) {
		uint64_t tmp = (counter++) << 48;
		uint64_t value = rdtsc() | tmp;
		*shm = value;
	}
}