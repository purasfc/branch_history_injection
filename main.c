#include <sys/syscall.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "common.h"

int main(void) {
	uint8_t *fr_buf;
	uint8_t *fr_buf_kern;
	fr_buf = mmap(NULL, STRIDE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE, -1, 0);
	if (fr_buf == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	memset(fr_buf, 0x41, STRIDE);
	fr_buf_kern = (uint8_t *)(virt_to_physmap((uint64_t) fr_buf));	
	munmap(fr_buf, STRIDE);
	return 0;
}
