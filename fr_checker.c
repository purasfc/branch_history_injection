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
volatile uint8_t evict_buffer[EVICT_SIZE];


static int indices[EVICT_SIZE / 64];
static int indices_init = 0;

void init_evict_indices(void) {
    int n = EVICT_SIZE / 64;
    for (int i = 0; i < n; i++)
        indices[i] = i * 64;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
    indices_init = 1;
}

uint8_t evict_target(void) {
    if (!indices_init) init_evict_indices();
    volatile uint8_t sum = 0;
    int n = EVICT_SIZE / 64;
    for (int i = 0; i < n; i++)
        sum += evict_buffer[indices[i]];
    return sum;
}

int main (int argc, char **argv) {
	uint64_t t;
	uint64_t avg, min, max;
	size_t results[2];
	memset((void*)evict_buffer, 0x42, EVICT_SIZE);
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
		evict_target();
		fence();

		t = load_time(fr_buf);
		printf("%lu\n", t);
		avg += t;
		if (t > max) max = t;
		if (t < min) min = t;
	}
	printf("Slow access: avg: %f min: %ld max: %ld\n", (float)avg/ITER, min, max);

	char *secret = "immature-asshole";
	uint8_t byte;

	for (int i=0; i<strlen(secret); i++) {
		byte = 0;
		for (int j=0; j<8; j++) {
			memset(results, 0, sizeof(results));

			for (int k=0; k<ITER; k++) {
				fence();
				evict_target();
				maccess(&fr_buf[((secret[i]>>j)&1)*STRIDE]);
				fence();
				reload(fr_buf, results);
			}
			printf("(%3ld %3ld) ", results[0], results[1]);

			add_bit_leak(&byte, j, results);
		}

		printf("\n%x %c\n\n", byte, byte);
	}

	munmap(fr_buf, 2*STRIDE);
	return 0;
}

