#define _GNU_SOURCE // for <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sched.h>

#include "../ctrlpf_ioctl.h"

// file descriptor to the device file that is our ioctl link to the kernel
// module. Initialized at program start.
static int ctrlpf_fd;


#ifdef __x86_64__
	// ===== Intel =====

	/**
	 * @brief      Sends the given msr_value to the kernel module to write it
	 *             into the Intel MSR.
	 *
	 * @param[in]  msr_value  The msr value
	 */
	void ioctl_intel_msrvalue(uint32_t msr_value) {
		int res = ioctl(ctrlpf_fd, IOCTL_CTRLPF_INTEL_MSRVALUE, msr_value);
		if (res != 0) {
			exit(-1);
		}
		printf("Sent msr_value %u\n", msr_value);
	}

#elif defined(__aarch64__) && ! defined(__APPLE__)
	// ===== ARM A72 =====

	/**
	 * @brief      Sends a "prefetch enable" request for ARM A72 to the
	 *             kernel module.
	 */
	void ioctl_arm_a72_enable(void) {
		int res = ioctl(ctrlpf_fd, IOCTL_CTRLPF_ARM_A72_ENABLE);
		if (res != 0) {
			exit(-1);
		}
		printf("Sent ARM A72 enable command.\n");
	}

	/**
	 * @brief      Sends a "prefetch disable" request for ARM A72 to the
	 *             kernel module.
	 */
	void ioctl_arm_a72_disable(void) {
		int res = ioctl(ctrlpf_fd, IOCTL_CTRLPF_ARM_A72_DISABLE);
		if (res != 0) {
			exit(-1);
		}
		printf("Sent ARM A72 disable command.\n");
	}
#endif

int main() {
	// open device file for communication with the kernel module
	ctrlpf_fd = open(DEVICE_FILE_PATH, 0);
	if (ctrlpf_fd < 0) {
		perror("Can't open device file " DEVICE_FILE_PATH);
		exit(-1);
	}

	printf("Running on core %d\n", sched_getcpu());

	#ifdef __x86_64__
		// ===== Intel =====
		// disable all prefetchers
		ioctl_intel_msrvalue(0b1111);
		// enable all prefetchers
		ioctl_intel_msrvalue(0);
	#elif defined(__aarch64__) && ! defined(__APPLE__)
		// ===== ARM A72 =====
		// disable all prefetchers
		ioctl_arm_a72_disable();
		// enable all prefetchers
		ioctl_arm_a72_enable();
	#endif

	close(ctrlpf_fd);
}