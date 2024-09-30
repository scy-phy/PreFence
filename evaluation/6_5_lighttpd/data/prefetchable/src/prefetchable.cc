#include <iostream>
#include <string>
#include <cinttypes>
#include <cstdlib>

#include "values.hh"

using std::string;

__attribute__((always_inline)) static inline uint64_t rdtsc() {
	#if defined(__i386__) || defined(__x86_64__)
		uint64_t a, d;
		asm volatile("mfence");
		asm volatile("rdtscp" : "=a"(a), "=d"(d) :: "rcx");
		a = (d << 32) | a;
		asm volatile("mfence");
		return a;
	#elif defined(__aarch64__)
		uint64_t result = 0;
		asm volatile("ISB");
		asm volatile("DSB SY");
		asm volatile("MRS %0, PMCCNTR_EL0" : "=r" (result));
		asm volatile("ISB");
		asm volatile("DSB SY");
		return result;
	#endif
}

int main() {
	srand(rdtsc());
	
	uint32_t col_idx = rand() % NO_COLUMNS;

	uint32_t min = 0xffffffff;
	for (size_t row_idx = 0; row_idx < NO_ROWS; ++row_idx) {
		uint32_t const& current = values[row_idx][col_idx];
		if (current < min) {
			min = current;
		}
	}

	std::cout << "Content-Type: text/html;charset=utf-8" << std::endl << std::endl;
	std::cout << "<!doctype html>\n"
		"<html>\n"
		"<head>\n"
		"<meta charset=\"utf-8\" />\n"
		"<title>Result</title>\n"
		"</head>\n"
		"<body>\n"
		"<h1>Result</h1>\n"
		"<table border=\"1\">\n"
		"<tr>\n"
		"<td>Selected column</td>\n"
		"<td><code>" << col_idx << "</code></td>\n"
		"</tr>\n"
		"<tr>\n"
		"<td>Minimum value</td>\n"
		"<td><code>" << min << "</code></td>\n"
		"</tr>\n"
		"</table>\n"
		"</body>\n"
		"</html>\n";

	return 0;
}
