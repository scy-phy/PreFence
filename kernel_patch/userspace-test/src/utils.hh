#pragma once

#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

#include <cassert>
#include <cstring>
#include <cstdint>

#include <string>

using std::string;

/**
 * Helper function that returns a cpu_set_t with a cpu affinity mask that
 * limits execution to the single (logical) CPU core.
 *
 * @param[in]  cpu   The number of the target CPU core
 *
 * @return     The cpuset.
 */
static inline cpu_set_t build_cpuset(int cpu) {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	return cpuset;
}

/**
 * Helper function that returns a cpu_set_t with a cpu affinity mask that
 * limits execution to two (logical) CPU cores.
 *
 * @param[in]  cpu1  The number of the first target CPU core
 * @param[in]  cpu2  The number of the second target CPU core
 *
 * @return     The cpuset.
 */
static inline cpu_set_t build_cpuset(int cpu1, int cpu2) {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu1, &cpuset);
	CPU_SET(cpu2, &cpuset);
	return cpuset;
}

/**
 * Set affinity mask of the given process so that the process is executed
 * on a specific (logical) core.
 *
 * @param[in]  pid   The pid of the process to move/pin (0 = self)
 * @param[in]  cpu   The number of the target CPU core
 *
 * @return     Return value of sched_setaffinity (in glibc: 0 on success,
 *             -1 otherwise)
 */
static inline int pin_process_to_cpu(pid_t pid, int cpu) {
	cpu_set_t cpuset = build_cpuset(cpu);
	return sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset);
}

// ======================== Intel MSR read/write ==========================

#define USE_CURRENT_CPU (-1)

/**
 * Returns the path for the MSR file of the specified CPU.
 *
 * @param[in]  cpu   The processor ID. If USE_CURRENT_CPU, the CPU the
 *                   current process is running on will be used.
 *
 * @return     The MSR file path.
 */
static inline string msr_file_path(int cpu) {
	if (cpu == USE_CURRENT_CPU) {
		cpu = sched_getcpu();
	}
	return "/dev/cpu/" + std::to_string(cpu) + "/msr";
}

/**
 * Read MSR on the specified CPU.
 *
 * @param[in]  cpu      The processor ID
 * @param[in]  msr_reg  The msr register
 *
 * @return     { description_of_the_return_value }
 */
static inline uint64_t rdmsr(int cpu, uint32_t msr_reg) {
	uint64_t msr_value;
	int fd = open(msr_file_path(cpu).c_str(), O_RDONLY);
	if (fd == -1) {
		printf("RDMSR open error: %s\n", strerror(errno));
		exit(1);
	}
	if (pread(fd, &msr_value, sizeof(msr_value), msr_reg) != sizeof(msr_value)) {
		printf("RDMSR pread error: %s\n", strerror(errno));
		exit(1);
	}
	close(fd);

	return msr_value;
}

/**
 * Write MSR on the specified CPU. Note that writing to MSR's might be
 * prohibited even with root privileges ("kernel lockdown"), especially on
 * systems using Secure Boot. See
 * https://man7.org/linux/man-pages/man7/kernel_lockdown.7.html for
 * details.
 *
 * @param[in]  cpu        The processor ID
 * @param[in]  msr_reg    The msr register
 * @param[in]  msr_value  The MSR value
 */
static inline void wrmsr(int cpu, uint32_t msr_reg, uint64_t msr_value) {
	int fd = open(msr_file_path(cpu).c_str(), O_WRONLY);
	if (fd == -1) {
		printf("WRMSR open error: %s\n", strerror(errno));
		exit(1);
	}
	if (pwrite(fd, &msr_value, sizeof(msr_value), msr_reg) != sizeof(msr_value)) {
		printf("WRMSR pwrite error: %s\n", strerror(errno));
		exit(1);
	}
	close(fd);
}

// Intel prefetcher control
// http://web.archive.org/web/20201125030055if_/https://software.intel.com/content/www/us/en/develop/articles/disclosure-of-hw-prefetcher-control-on-some-intel-processors.html
// Register:
#define INTEL_MSR_MISC_FEATURE_CONTROL (0x1A4)
/**
 * Enum to keep the bit masks to select the prefetcher we want to control
 * via MSR_MISC_FEATURE_CONTROL register on Intel CPUs.
 */
enum intel_prefetcher_t {
	INTEL_L2_HW_PREFETCHER          = 0b0001ULL,
	INTEL_L2_ADJACENT_CL_PREFETCHER = 0b0010ULL,
	INTEL_DCU_PREFETCHER            = 0b0100ULL,
	INTEL_DCU_IP_PREFETCHER         = 0b1000ULL,
};

/**
 * Enables or disables a specific prefetcher on Intel CPUs.
 *
 * @param[in]  cpu         The processor ID
 * @param[in]  prefetcher  The prefetcher to enable/disable.
 * @param[in]  enabled     true to enable, false to disable the prefetcher
 */
static inline void set_intel_prefetcher(int cpu, intel_prefetcher_t prefetcher, bool enabled) {
	uint32_t msr_reg = INTEL_MSR_MISC_FEATURE_CONTROL;
	uint64_t msr_value = rdmsr(cpu, msr_reg);
	if ( ! enabled) {
		msr_value |= prefetcher;
	} else {
		msr_value &= ~(prefetcher);
	}
	printf("set_intel_prefetcher: Writing 0x%" PRIx64 " into 0x%" PRIx32 ".\n", msr_value, msr_reg);
	wrmsr(cpu, msr_reg, msr_value);
	assert(rdmsr(cpu, msr_reg) == msr_value);
}

/**
 * Enables or disables a specific prefetcher on Intel CPUs.
 *
 * @param[in]  cpu         The processor ID
 * @param[in]  prefetcher  The prefetcher to enable/disable.
 * @param[in]  enabled     true to enable, false to disable the prefetcher
 */
static inline uint64_t get_intel_prefetcher(int cpu) {
	uint32_t msr_reg = INTEL_MSR_MISC_FEATURE_CONTROL;
	return rdmsr(cpu, msr_reg);
}
