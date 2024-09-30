#define _GNU_SOURCE

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>

#include <sys/ioctl.h>

#include <sys/prctl.h>
#define PR_SET_PREFETCH_DISABLE         128
#define PR_GET_PREFETCH_DISABLE         129

#include "../pfstate_ioctl.h"

void print(int state) {
	printf("Value: %d\n", state);
	if (state & PFSTATE_DISABLE_LOAD_STORE_HW_PF) {
		printf(" - %s\n", PFSTATE_STR_DISABLE_LOAD_STORE_HW_PF);
	}
	if (state & PFSTATE_L1_L2_PREFETCH_CROSS_4KB) {
		printf(" - %s\n", PFSTATE_STR_L1_L2_PREFETCH_CROSS_4KB);
	}
	if (state & PFSTATE_LOAD_STORE_HW_PF_USE_VA) {
		printf(" - %s\n", PFSTATE_STR_LOAD_STORE_HW_PF_USE_VA);
	}
	if (state & PFSTATE_DISABLE_PF_READUNIQUE) {
		printf(" - %s\n", PFSTATE_STR_DISABLE_PF_READUNIQUE);
	}
	if (state & PFSTATE_DISABLE_L1I_PREFETCH) {
		printf(" - %s\n", PFSTATE_STR_DISABLE_L1I_PREFETCH);
	}
	if (state & PFSTATE_DISABLE_L2_TLB_PREFETCH) {
		printf(" - %s\n", PFSTATE_STR_DISABLE_L2_TLB_PREFETCH);
	}
}

int main() {
	int ret = prctl(PR_SET_PREFETCH_DISABLE, 1, NULL, NULL);
	if (ret < 0) { perror("  Error"); }
	if (ret != 0) {
		printf("ERROR: set_flag failed. Error code: %d\n", ret);
		return -1;
	}

	// open device file for communication with the kernel module
	int pfstate_dev_fd = open("/dev/" DEVICE_FILE_NAME, 0);
	if (pfstate_dev_fd < 0) {
		printf ("Can't open device file: %s\n", DEVICE_FILE_NAME);
		return -1;
	}

	int state = ioctl(pfstate_dev_fd, IOCTL_QUERY, sched_getcpu());
	print(state);

	close(pfstate_dev_fd);

	return 0;
}