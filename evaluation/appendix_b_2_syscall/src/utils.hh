#pragma once

#include <cinttypes>
#if defined(INTEL_CLOCK_SOURCE) && (INTEL_CLOCK_SOURCE == INTEL_CLOCK_MONOTONIC)
	#include <time.h>
#endif

#include <cstdlib>
#include <cstdio>

#include <unistd.h>
#include <sched.h>

#include "params.hh"

__attribute__((always_inline)) static inline uint64_t rdtsc() {
	#if defined(__i386__) || defined(__x86_64__)
		#if INTEL_CLOCK_SOURCE == INTEL_CLOCK_RDTSCP
			uint64_t a, d;
			asm volatile("mfence");
			asm volatile("rdtscp" : "=a"(a), "=d"(d) :: "rcx");
			a = (d << 32) | a;
			asm volatile("mfence");
			return a;
		#elif INTEL_CLOCK_SOURCE == INTEL_CLOCK_MONOTONIC
			asm volatile("mfence");
			struct timespec t1;
			clock_gettime(CLOCK_MONOTONIC, &t1);
			uint64_t res = t1.tv_sec * 1000 * 1000 * 1000ULL + t1.tv_nsec;
			asm volatile("mfence");
			return res;
		#else
			#error "Unknown clock primitive"
		#endif
	#elif defined(__aarch64__)
		uint64_t result = 0;
		asm volatile("ISB");
		asm volatile("DSB SY");
		asm volatile("MRS %0, PMCCNTR_EL0" : "=r" (result));
		asm volatile("ISB");
		asm volatile("DSB SY");
		return result;
	#else
		#error "No timing source available for your architecture"
	#endif
}

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