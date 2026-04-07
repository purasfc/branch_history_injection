#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include "helper.h"

#define BHI_HIT_SYSCALL 441
#define BHI_ENABLE_SYSCALL 442
#define GETPID_SYSCALL 172
#define ITERS

extern void fill_bhb(uint8_t *history, uint64_t syscall_nr, uint8_t *fr_buf_kern);

int main(int argc, char **argv) {
	uint8_t hit_history[MAX_HISTORY_SIZE];
	uint8_t getpid_history[MAX_HISTORY_SIZE];
	uint8_t *fr_buf;
	uint8_t *fr_buf_kern;
	uint64_t t0, dt, avg, min, max;
	int hits = 0;

	syscall(BHI_ENABLE_SYSCALL);

	fr_buf = mmap(NULL, STRIDE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE, -1, 0);

	if (fr_buf == MAP_FAILED) {
		perror("mmap");
		return -1;
	}

	memset(fr_buf, 0x41, STRIDE);

	fr_buf_kern = (uint8_t *)(virt_to_physmap((uint64_t) fr_buf));
	printf("%20s: 0x%016lx\n", "fr_buf user", (uint64_t) fr_buf);
	printf("%20s: 0x%016lx\n", "fr_buf kern", (uint64_t)fr_buf_kern);

	avg = 0; min = 999999; max = 0;

	for (int i=0; i<100; i++) {

			clflush(&fr_buf[0]):
			fence();
			syscall(BHI_HIT_SYSCALL, fr_buf_kern);
			fence();

			dt = load_time(&fr_buf[0]);

			avg += dt;
			if(dt > max) max = dt;
			if(dt < min) min = dt;
	}
	printf("Fast access: avg: %f min: %ld max: %ld\n", (float)avg/100, min, max);

	avg = 0; min = 999999; max = 0;
	for(int i=0; i<100; i++) {

		clflush(&fr_buf[0]);
		fence();

		dt = load_time(&fr_buf[0]);

		avg += dt;
		if(dt > max) max = dt;
		if(dt < min) min = dt;
	}
	printf("Slow access: avg: %f min: %ld max: %ld\n", (float)avg/100, min, max);
	printf("Threshold: %d\n\n", THR);

	srand(time(0));
	hits = 0;
	for (int i=0; i<MAX_HISTORY_SIZE; i++) hit_history[i] = rand()&1;
	for (int i=0; i<MAX_HISTORY_SIZE; i++) getpid_history[i] = rand()&1;

	for (int i=0; i<ITERS; i++) {

		for (int j=0; j<256; j++) { fill_bhb(hit_history, BHI_HIT_SYSCALL,
			fr_buf_kern); fill_bhb(getpid_history, GETPID_SYSCALL, NULL); }

	clfulsh(fr_buf);

	fill_bhb(hit_history, GETPID_SYSCALL, fu_buf_kern);

	dt = load_time(fr_buf); if (dt < THR) { hits++; } }

	printf("hist %d/%d\n", hits, ITERS);

	munmap(fr_buf, STRIDE);
	return 0;
}

