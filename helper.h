#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include "helper.h"
#include "targets.h"

static inline __attribute__((always_inline)) void fence(void) {
	asm volatile("dsb ish");
	asm volatile("isb");
}

static inline __attribute__((always_inline)) void maccess(void *p) {
	*(volatile unsigned char *)p;
}

static inline __attribute__((always_inline)) void clflush(void *p) {
	asm volatile("dc civac, %0" :: "r"(p));
	asm volatile("dsb ish");
	asm volatile("isb");
}

static inline __attribute__((always_inline)) uint64_t rdtscp(void) {
	uint64_t result;
	fence();
	asm volatile("mrs %0, cntvct_el0" : "=r" (result));
	fence();
	return result;
}

static inline __attribute__((always_inline)) uint64_t load_time(void *p) {
	uint64_t t0 = rdtscp();
	maccess(p);
	return rdtscp() - t0;
}

uint64_t virt_to_physmap(uint64_t virtual_address) {
	int pagemap;
	uint64_t value;
	int got;
	uint64_t page_frame_number;

	pagemap = open("/proc/self/pagemap", O_RDONLY);
	if (pagemap < 0) {
		exit(1);
	}

	got = pread(pagemap, &value, 8, (virtual_address / 0x1000) * 8);
	if (got != 8) {
		exit(2);
	}

	page_frame_number = value & ((1ULL << 54) - 1);
	if (page_frame_number == 0) {
		exit(3);
	}

	close(pagemap);

	return ((page_frame_number*0x1000) - PHYS_OFFSET) | PAGE_OFFSET;
}

#endif // _HELPER_H_

