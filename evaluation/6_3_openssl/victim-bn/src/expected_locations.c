#include <inttypes.h>
#include <stdio.h>
#include <openssl/bn.h>

int main() {
	uint64_t w = 0x2468ace;
	printf("Expected hit locations:\n");
	printf("SQR1:\n");
	printf(" %lu\n", ((w) >> 60 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 56 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 52 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 48 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 44 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 40 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 36 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 32 & 0xF) * sizeof(BN_ULONG));
	printf("SQR0:\n");
	printf(" %lu\n", ((w) >> 28 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 24 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 20 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 16 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >> 12 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >>  8 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w) >>  4 & 0xF) * sizeof(BN_ULONG));
	printf(" %lu\n", ((w)       & 0xF) * sizeof(BN_ULONG));
}