#pragma once

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>

#ifndef PAGE_SIZE
	#define PAGE_SIZE (4096)
#endif
#ifndef CACHE_LINE_WIDTH
	#define CACHE_LINE_WIDTH (64)
#endif

typedef struct {
	uint8_t* base_addr;
	size_t size;
} Mapping;

typedef struct Result {
	size_t index;
	uint64_t time;
} Result;

// Initialize timing cycle count register on the current CPU core
void timing_init() {}


#if defined(__i386__) || defined(__x86_64__)
	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline uint64_t rdtsc() {
		uint64_t a, d;
		asm volatile("mfence");
		asm volatile("rdtscp" : "=a"(a), "=d"(d) :: "rcx");
		a = (d << 32) | a;
		asm volatile("mfence");
		return a;
	}

	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline void flush(void *p) { asm volatile("clflush 0(%0)\n" : : "c"(p) : "rax"); }

	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline void maccess(void *p) { asm volatile("movq (%0), %%rax\n" : : "c"(p) : "rax"); }

	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline void mfence() { asm volatile("mfence"); }
#elif defined(__aarch64__)
	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline uint64_t rdtsc() {
		uint64_t result = 0;
		asm volatile("ISB");
		asm volatile("DSB SY");
		asm volatile("MRS %0, PMCCNTR_EL0" : "=r" (result));
		asm volatile("ISB");
		asm volatile("DSB SY");
		return result;
	}

	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline void flush(void *p) {
		asm volatile("DC CIVAC, %0" ::"r"(p));
		asm volatile("DSB ISH");
		asm volatile("ISB");
	}

	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline void maccess(void *p) {
		volatile uint32_t value;
		asm volatile("LDR %0, [%1]\n\t" : "=r"(value) : "r"(p));
		// asm volatile("DSB ISH");
		// asm volatile("ISB");
	}

	// ---------------------------------------------------------------------------
	__attribute__((always_inline)) static inline void mfence() { asm volatile("DSB ISH"); }
#else
	#error "Architecture not supported."
#endif

static inline __attribute__((always_inline)) int reload_t(void *ptr) {
	uint64_t start = 0, end = 0;

	mfence();
	start = rdtsc();
	mfence();
	maccess(ptr);
	mfence();
	end = rdtsc();
	mfence();

	return (int)(end - start);
}

static inline __attribute__((always_inline)) int reload_flush_t(void *ptr) {
	uint64_t start = 0, end = 0;

	mfence();
	start = rdtsc();
	mfence();
	maccess(ptr);
	mfence();
	end = rdtsc();
	mfence();
	flush(ptr);
	mfence();

	return (int)(end - start);
}

static inline __attribute__((always_inline)) int flush_t(void *ptr) {
	uint64_t start = 0, end = 0;

	mfence();
	start = rdtsc();
	mfence();
	flush(ptr);
	mfence();
	end = rdtsc();
	mfence();

	return (int)(end - start);
}

// Helper function that returns a cpu_set_t with a cpu affinity mask
// that limits execution to the single (logical) CPU core cpu.
cpu_set_t build_cpuset(int cpu) {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	return cpuset;
}

// Set affinity mask of the given process so that the process is executed
// on a specific (logical) core.
int move_process_to_cpu(pid_t pid, int cpu) {
	cpu_set_t cpuset = build_cpuset(cpu);
	return sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset);
}
