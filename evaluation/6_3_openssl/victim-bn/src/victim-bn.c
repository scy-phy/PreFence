#define _GNU_SOURCE

#include <inttypes.h>
#include <stdbool.h>

#ifdef COUNT_MISSES
	// performance counters
	#include <linux/perf_event.h>
	#include <sys/ioctl.h>
	#include <asm/unistd.h>
#endif // COUNT_MISSES

// prefetch control
#include <sys/prctl.h>
#define PR_SET_PREFETCH_DISABLE         128
#define PR_GET_PREFETCH_DISABLE         129

// OpenSSL
#include <openssl/bn.h>

#include "common_test.h"

// The following offsets are used to compute the address of SQR_tb at
// runtime. Since OpenSSL does not export SQR_tb as a symbol, there is no
// way to find the address directly. Therefore, I inspected libcrypto.so
// (objdump -D libcrypto.so), extracted the offset of an exported symbol as
// reference and the offset of SQR_tb.
#define OFFSET_BN_GCD (0x000000000009f3c0) // objdump -D openssl/libcrypto.so.1.1  | grep "<BN_gcd>:"
#define OFFSET_SQR_TB (0x000000000019f0e0) // objdump -D openssl/libcrypto.so.1.1  | grep "<SQR_tb>:"

// sum of all elements in SQR_tb should be 680. Will be used to verify that
// the computed address is correct.
#define SQR_TB_EXPECTED_SUM (680)

// Number of repetitions to perform
#define NO_REPETITIONS (10000000)

// Which CPU to run on
#define EXPERIMENT_CPU 3

// Structs for nanosleep
struct timespec const t_req = { .tv_sec = 0, .tv_nsec = 1000 /* 1µs */ };
struct timespec t_rem = {0};

BIGNUM* make_bn() {
	BIGNUM* bn;
	if (NULL == (bn = BN_new())) {
		fprintf(stderr, "Could not create bignum\n");
		exit(1);
	}
	return bn;
}

#ifdef COUNT_MISSES
	long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags) {
		int ret;
		ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
		return ret;
	}
#endif // COUNT_MISSES

int main(int argc, char** argv) {
	move_process_to_cpu(0, EXPERIMENT_CPU);
	printf("CPU: %d\n", sched_getcpu());
	printf("NO_REPETITIONS: %d\n", NO_REPETITIONS);

	#ifdef COUNT_MISSES
		// Open file descriptor to read performance counters
		struct perf_event_attr event_attr;
		int64_t event_count;
		int event_fd;
		memset(&event_attr, 0, sizeof(struct perf_event_attr));
		event_attr.type = PERF_TYPE_HARDWARE;
		event_attr.size = sizeof(struct perf_event_attr);
		event_attr.config = PERF_COUNT_HW_CACHE_MISSES; //PERF_COUNT_HW_INSTRUCTIONS;
		event_attr.disabled = 1;
		event_attr.exclude_kernel = 1;
		event_attr.exclude_hv = 1;
		event_fd = perf_event_open(&event_attr, 0, EXPERIMENT_CPU, -1, 0);
		if (event_fd == -1) {
			fprintf(stderr, "Error opening leader %llx\n", event_attr.config);
			exit(EXIT_FAILURE);
		}
	#endif // COUNT_MISSES

	// Compute address of SQR_tb
	uint8_t* const SQR_tb_ptr = ((uint8_t*)&BN_gcd) - OFFSET_BN_GCD + OFFSET_SQR_TB;
	uint8_t* const SQR_tb_cl_begin = (uint8_t*)((uintptr_t)SQR_tb_ptr & ~0b111111);
	printf("SQR_tb_ptr:        %p\n", SQR_tb_ptr);
	printf("SQR_tb_ptr offset from cache line begin: %zu\n", (SQR_tb_ptr - SQR_tb_cl_begin));

	// verify the computed address by computing the sum of all elements and
	// comparing it with the expected value.
	{
		uint64_t sum = 0;
		for (size_t i = 0; i < 16; i++) {
			sum += ((uint64_t*)SQR_tb_ptr)[i];;
		}
		if (sum != SQR_TB_EXPECTED_SUM) {
			fprintf(stderr, "SQR_tb did not pass sum validation check. Please check OFFSET_BN_GCD and OFFSET_SQR_TB.\n");
			return 1;
		} else {
			printf("SQR_tb passed validation check.\n");
		}
	}

	// cache lines to probe
	uint8_t* const lines_to_probe[] = {
		// lines before SQR_tb
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) - 7*64), // 0
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) - 6*64), // 1
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) - 5*64), // 2
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) - 4*64), // 3
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) - 3*64), // 4
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) - 2*64), // 5
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) - 1*64), // 6
		
		// lines containing SQR_tb
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin)),       // 7
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 64),  // 8

		// lines after SQR_tb
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128), // 9
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128 + 1*64), // 10
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128 + 2*64), // 11
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128 + 3*64), // 12
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128 + 4*64), // 13
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128 + 5*64), // 14
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128 + 6*64), // 15
		(uint8_t*)(((uintptr_t)SQR_tb_cl_begin) + 128 + 7*64), // 16
	};
	// number of elements in lines_to_probe
	size_t n_lines_to_probe = sizeof(lines_to_probe)/sizeof(lines_to_probe[0]);

	// ensure we have at least one command line argument
	if (argc < 2) {
		fprintf(stderr,
			"Please provide a hexadecimal number to put into bn1 as first "
			"command line parameter. Do not use any prefix (like '0x').\n"
			"To trigger prefetching, try 02468ace. "
			"To not trigger prefetching, try 02020202.\n"
			"Optionally provide the number of a cache line to probe as "
			"second command line parameter.\n"
			"Usually, lines 9 or 10 are a good choice.\n"
		);
		return 1;
	}

	// if second command line argument (cache line no. to probe), parse and
	// store it.
	ssize_t line_no = -1;
	if (argc >= 3) {
		line_no = atoll(argv[2]);
	}

	// set up openssl bignum context
	BN_CTX* bn_ctx;
	bn_ctx = BN_CTX_new();
	if (NULL == (bn_ctx = BN_CTX_new())) {
		fprintf(stderr, "Error creating the context.\n");
		return 2;
	}

	// create two BIGNUMs
	BIGNUM* bn1 = make_bn();
	BIGNUM* bn2 = make_bn();
	
	// set bn1 to a specific value (provided by command line argument 1)
	{
		int ret = BN_hex2bn(&bn1, argv[1]);
		if (ret == 0) {
			// error
			fprintf(stderr, "could not parse command line argument 1 as BIGNUM.\n");
			return 1;
		} else {
			// success; echo the resulting value of bn1.
			char* scalar_str = BN_bn2hex(bn1);
			printf("bn hex value: %s\n", scalar_str);
			OPENSSL_free(scalar_str);
		}
	}

	// parameter p is not really relevant for our experiment since it is not involved
	// in the lookup address computation. Just set it to any value that does not crash
	// the program
	const int p[] = {0};

	for (int use_countermeasure = 0; use_countermeasure <= 1; use_countermeasure++) {
		for (size_t repetition = 0; repetition < NO_REPETITIONS; repetition++) {
			uint64_t time_compute = 0;
			uint64_t time_reload = 0;
			#ifdef COUNT_MISSES
				int64_t misses = 0;
				// reset cache miss performance counter
				ioctl(event_fd, PERF_EVENT_IOC_RESET, 0);
			#endif // COUNT_MISSES
	
			// before the actual function call to trace, ensure the cache lines
			// around SQR_tb are uncached (if we are probing)
			if (line_no >= 0) {
				for (size_t i = 0; i < n_lines_to_probe; i++) {
					flush(lines_to_probe[i]);
				}
				mfence();
			}
	
			// call the openssl function to trace
			{
				// set prefetch_disable flag (if PreFence is enabled)
				if (use_countermeasure) {
					int ret = prctl(PR_SET_PREFETCH_DISABLE, 1, NULL, NULL);
					if (ret < 0) { perror("prctl failed"); exit(EXIT_FAILURE); }
					if (ret != 0) { printf("prctrl returned unexpected value %d\n", ret); exit(EXIT_FAILURE); }
				}
	
				#ifdef COUNT_MISSES
					// start cache miss performance counter
					ioctl(event_fd, PERF_EVENT_IOC_ENABLE, 0);
				#endif // COUNT_MISSES
	
				// take a timestamp before the action
				uint64_t time_compute_before = rdtsc();
	
				// PERFORM THE ACTION
				int ret = BN_GF2m_mod_sqr_arr(bn2, bn1, p, bn_ctx);
	
				// take a timestamp after the action and compute the runtime
				time_compute = rdtsc() - time_compute_before;
	
				#ifdef COUNT_MISSES
					// stop cache miss performance counter and read the value
					ioctl(event_fd, PERF_EVENT_IOC_DISABLE, 0);
					read(event_fd, &misses, sizeof(misses));
				#endif
	
				// clear prefetch_disable flag (if PreFence is enabled)
				if (use_countermeasure) {
					int ret = prctl(PR_SET_PREFETCH_DISABLE, 0, NULL, NULL);
					if (ret < 0) { perror("prctl failed"); exit(EXIT_FAILURE); }
					if (ret != 0) { printf("prctrl returned unexpected value %d\n", ret); exit(EXIT_FAILURE); }
				}
	
				if (ret != 1) {
					fprintf(stderr, "Error in BN_GF2m_mod_sqr_arr, error code: %d\n", ret);
				}
			}
			nanosleep(&t_req, &t_rem);
			// cache inspection
			if (line_no >= 0 && line_no < sizeof(lines_to_probe)/sizeof(lines_to_probe[0])) {
				mfence();
				time_reload = reload_t(lines_to_probe[line_no]);
			}
			#ifdef COUNT_MISSES
				printf("L;%u;%ld;%lu;%lu\n", use_countermeasure, misses, time_compute, time_reload);
			#else
				printf("L;%u;%ld;%lu;%lu\n", use_countermeasure, -1L, time_compute, time_reload);
			#endif // COUNT_MISSES
		}
	}

	// cleanup
	BN_free(bn1);
	BN_free(bn2);
	BN_CTX_free(bn_ctx);

	#ifdef COUNT_MISSES
		close(event_fd);
	#endif // COUNT_MISSES
	
	return 0;
}

