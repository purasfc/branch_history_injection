#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include "targets.h"
#include "flush_reload.h"

#define ITER 100
uint8_t evict_buffer[EVICT_SIZE];

void evict_target(void) {
	uint8_t sum = 0;
	for (int i=0; i<EVICT_SIZE; i++) {
		sum += evict_buffer[i];
	}
}

int main (int argc, char **argv) {
	uint64_t t;
	uint64_t avg, min, max;
	size_t results[2];
	uint8_t *fr_buf = mmap(NULL, 2*STRIDE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE, -1, 0);
	printf("fr_buf = %p\n", fr_buf);
	memset(fr_buf, 0x41, 2*STRIDE);

	avg = 0; min = 999999; max = 0;
	for (int i=0; i<ITER; i++) {
		maccess(&fr_buf[0]);
		fence();

		t = load_time(fr_buf);
		avg += t;
		if (t > max) max = t;
		if (t < min) min = t;
	}
	printf("Fast access: avg: %f min: %ld max: %ld\n", (float)avg/ITER, min, max);

	avg = 0; min = 999999; max = 0;
	for (int i=0; i<ITER; i++) {
		clflush(fr_buf);
		fence();

		t = load_time(fr_buf);
		avg += t;
		if (t > max) max = t;
		if (t < min) min = t;
	}
	printf("Slow access: avg: %f min: %ld max: %ld\n", (float)avg/ITER, min, max);

	munmap(fr_buf, 2*STRIDE);
	return 0;
}

