#include <stdio.h>
#include <stdint.h>

int main() {
	uint64_t v1, v2;
	asm volatile("mrs %0, pmccntr_el0" : "=r"(v1));
	asm volatile("mrs %0, pmccntr_el0" : "=r"(v2));
	printf("v1=%lu v2=%lu diff=%lu\n", v1, v2, v2-v1);
	return 0;
}

