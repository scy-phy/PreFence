#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <inttypes.h>
#include "utils.hh"

// Since linux kernel headers in /usr/include/linux are managed separately
// from the kernel sources in /usr/src/linux (they are part of the
// kernel-headers package), we just re-define the additional options here:
#define PR_SET_PREFETCH_DISABLE		128
#define PR_GET_PREFETCH_DISABLE		129

#define TRY_FORK 1
#define TRY_CORE_MIGRATION 1
#define TRY_CLEAR_FLAG 1
//#define PRINT_TIMESTAMPS 1

#define CPU_INITIAL 0
#define CPU_MIGRATION 2

__attribute__((always_inline)) static inline void serialize(void) {
	asm volatile(
		"cpuid"
		: // no inputs
		: // no outputs
		: "eax", "ebx", "ecx", "edx" // clobbered
	);
}


__attribute__((always_inline)) static inline void print_timestamp(void) {
	struct timespec ts;
	int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
	if (ret != 0) {
		printf("Error in clock_gettime\n");
		exit(EXIT_FAILURE);
	}
	printf("Time: %lu.%lu\n", ts.tv_sec, ts.tv_nsec);
}

__attribute__((always_inline)) static inline void assert_pf(uint64_t expected) {
	#ifdef PRINT_TIMESTAMPS
		print_timestamp();
	#endif
	//serialize();
	uint64_t pfstate = get_intel_prefetcher(USE_CURRENT_CPU);
	printf("Prefetcher state: %" PRIu64 "\n", pfstate);
	#ifdef PRINT_TIMESTAMPS
		print_timestamp();
	#endif
	if (pfstate != expected) {
		printf("ERROR: expected pfstate %" PRIu64 ", was %" PRIu64 "\n", expected, pfstate);
		exit(EXIT_FAILURE);
	}
}

__attribute__((always_inline)) static inline void assert_flag(int expected) {
	//serialize();
	int flag = prctl(PR_GET_PREFETCH_DISABLE, NULL, NULL, NULL);
	printf("Prefetch disable flag: %d\n", flag);
	if (flag < 0) { perror("  Error"); }
	if (flag != expected) {
		printf("ERROR: expected flag %d, was %d\n", expected, flag);
		exit(EXIT_FAILURE);
	}
}

__attribute__((always_inline)) static inline void set_flag(int new_value) {
	#ifdef PRINT_TIMESTAMPS
		print_timestamp();
	#endif
	int ret = prctl(PR_SET_PREFETCH_DISABLE, new_value, NULL, NULL);
	//serialize();
	printf("Set prefetch disable flag to %d: %d\n", new_value, ret);
	#ifdef PRINT_TIMESTAMPS
		print_timestamp();
	#endif
	if (ret < 0) { perror("  Error"); }
	if (ret != 0) {
		printf("ERROR: set_flag failed. Error code: %d\n", ret);
		exit(EXIT_FAILURE);
	}
}

int main(void) {
	print_timestamp();
	pin_process_to_cpu(0, CPU_INITIAL);
	printf("Running on CPU %d, PID: %d\n", sched_getcpu(), getpid());

	// Check the current PF disable flag (should be 0 initially)
	assert_flag(0);
	// Check the current prefetcher state (should be 0 initially)
	assert_pf(0);
	puts("");

	// Set the PF disable flag
	set_flag(1);

	// Sleep to enforce a context switch
	//sched_yield();
	//sleep(1);

	// Check the current PF disable flag (should now be 1)
	assert_flag(1);
	// Check the current prefetcher state (should now be != 0)
	assert_pf(0b1111);
	puts("");

	#ifdef TRY_FORK
		// Check flag propagation to forks
		puts("[PARENT] Forking...");
		pid_t pid = fork();
		switch(pid) {
		case 0: // child
			printf("[CHILD] Running on CPU %d, PID: %d\n", sched_getcpu(), getpid());
			
			// Sleep to enforce a context switch
			//sched_yield();
			//sleep(1);
			
			// Check the child's PF disable flag (should be 1, as it should propagate)
			printf("[CHILD] ");
			assert_flag(1);
	
			// Check the current prefetcher state on the child's CPU (should be != 0)
			printf("[CHILD] ");
			assert_pf(0b1111);
	
			exit(EXIT_SUCCESS);
			break;
		case -1: // error
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		default: // parent: wait for child to terminate
			int wstatus;
			printf("[PARENT] Waiting for child %d...\n", pid);
			waitpid(pid, &wstatus, 0);
			int child_exit_code = WEXITSTATUS(wstatus);
			puts("[PARENT] Child done.");
			if (child_exit_code != 0) {
				printf("[PARENT] ERROR: Child failed with exit code %d.\n", child_exit_code);
				exit(EXIT_FAILURE);
			}
			puts("");
			break;
		}
	#endif // TRY_FORK
	
	#ifdef TRY_CORE_MIGRATION
		// Try a core migration
		printf("Migrating...\n");
		pin_process_to_cpu(0, CPU_MIGRATION);
		printf("Now running on CPU %d, PID: %d\n", sched_getcpu(), getpid());
	
		// Sleep to enforce a context switch
		//sched_yield();
		//sleep(1);
	
		assert_pf(0b1111);
	#endif // TRY_CORE_MIGRATION
	
	#ifdef TRY_CLEAR_FLAG
		// Clear the PF disable bit
		set_flag(0);
	
		// Sleep to enforce a context switch
		//sched_yield();
		//sleep(1);
	
		// Check the current PF disable flag (should now be 0 again)
		assert_flag(0);
		// Check the current prefetcher state (should now be 0 again)
		assert_pf(0);
	#endif // TRY_CLEAR_FLAG

	return EXIT_SUCCESS;
}
