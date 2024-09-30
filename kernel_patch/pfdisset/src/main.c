#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/prctl.h>

// Since linux kernel headers in /usr/include/linux are managed separately
// from the kernel sources in /usr/src/linux (they are part of the
// kernel-headers package), we just re-define the additional options here:
#define PR_SET_PREFETCH_DISABLE		128
#define PR_GET_PREFETCH_DISABLE		129

// Prefetch disable flag values
typedef enum {
	PFDISFLAG_PF_ENABLE = 0,
	PFDISFLAG_PF_DISABLE = 1,
	PFDISFLAG_UNKNOWN = 2
} pfdisflag_t;

void set_flag(int new_value) {
	int ret = prctl(PR_SET_PREFETCH_DISABLE, new_value, NULL, NULL);
	printf("[PFDISSET] Set prefetch disable flag to %d: %d\n", new_value, ret);
	if (ret < 0) {
		perror("[PFDISSET] Error");
	}
	if (ret != 0) {
		printf("[PFDISSET] ERROR: set_flag failed. Error code: %d\n", ret);
		exit(EXIT_FAILURE);
	}
}

void assert_flag(int expected) {
	int flag = prctl(PR_GET_PREFETCH_DISABLE, NULL, NULL, NULL);
	printf("[PFDISSET] Prefetch disable flag: %d\n", flag);
	if (flag < 0) {
		perror("[PFDISSET] Error");
	}
	if (flag != expected) {
		printf("[PFDISSET] ERROR: expected flag %d, was %d\n", expected, flag);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[]) {
	pfdisflag_t pfdisflag = PFDISFLAG_UNKNOWN;

	// Handle command line arguments.
	// - argv[0] is our own name (pfdisset)
	// - argv[1] sets the prefetch disable flag value (either -d or -e)
	// - argv[2] is the target program
	// - argv[3]+ are parameters for the target program
	if (argc <= 2) {
		printf("Usage: %s <-d|-e> <program> <args...>\n", argv[0]);
		puts(" -d: disable prefetching");
		puts(" -e: enable prefetching");
		return EXIT_FAILURE;
	} else if (argc > 2) {
		if (strcmp(argv[1], "-d") == 0) {
			pfdisflag = PFDISFLAG_PF_DISABLE;
		} else if (strcmp(argv[1], "-e") == 0) {
			pfdisflag = PFDISFLAG_PF_ENABLE;
		} else {
			fprintf(stderr, "Invalid parameter %s\n", argv[1]);
			return EXIT_FAILURE;
		}
	}

	// Set and assert the prefetch disable flag
	set_flag(pfdisflag);
	assert_flag(pfdisflag);

	// Hand over to the target program
	int ret = execvp(argv[2], &argv[2]);
	if (ret != 0) {
		perror("[PFDISSET] execvp failed");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}