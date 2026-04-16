#ifndef _FLUSH_RELOAD_H_
#define _FLUSH_RELOAD_H_

#include <ctype.h>
#include <stdint.h>
#include "targets.h"
#include "helper.h"

static inline __attribute__((always_inline)) void flush(uint8_t *reloadbuffer) {
	clflush(reloadbuffer + 0*STRIDE);
	clflush(reloadbuffer + 1*STRIDE);
	fence();
}

static inline __attribute__((always_inline)) void reload(unsigned char *reloadbuffer, size_t *results) {
	fence();

	for (size_t k=0; k<2; k++) {
		unsigned char *p = reloadbuffer + (STRIDE * k);

		if (load_time(p) < THR) {
			results[k]++;
		}
	}
}

static inline __attribute__((always_inline)) void add_bit_leak(uint8_t *byte, int pos, size_t *results) {
	if (results[1] > results[0]) {
		*byte = (*byte) | (1<<pos);
	}
}

#endif // _FLUSH_RELOAD_H_

