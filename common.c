#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "common.h"

// Get corresponding physmap address in kernel space
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
