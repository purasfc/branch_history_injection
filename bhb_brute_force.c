#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <time.h>
#include "targets.h"
#include "helper.h"

#define HISTORY_SIZE        64
#define ITER                100
#define IN_PLACE            1

#define B_OPCODE            (0x14000000U)
#define B_OFF_MASK_OPC      (0x03ffffffU)
#define B_OFF_MASK          (0x01ffffffU)
#define MAX_MEM             (0x04000000<<2)
#define BHI_ENABLE_SYSCALL  442

typedef uint64_t (*t_chain)(void *target, void *arg);
typedef void (*t_target)(void *target);
extern void ret_gadget(void *arg);
extern void hit_gadget(void *arg);
extern void ind_gadget(void *arg);

void jit(uint8_t *mem, uint64_t *history) {
	for (int i=0; i<HISTORY_SIZE-1; i++) {
		uint32_t offset = (history[i+1]>>2) - (history[i]>>2);
		uint32_t opcode = B_OPCODE | (offset & B_OFF_MASK_OPC);
		memcpy(&mem[history[i]], &opcode, 4);
		__clear_cache(&mem[history[i]], &mem[history[i]+4]);
	}
	memcpy(&mem[history[HISTORY_SIZE-1]], &ind_gadget, 8);
	__clear_cache(&mem[history[HISTORY_SIZE-1]], &mem[history[HISTORY_SIZE-1]+8]);
}

int main (int argc, char **argv) {
	uint8_t *fr_buf;
	uint8_t *mem;
	uint64_t hit_history[HISTORY_SIZE];
	uint64_t ret_history[HISTORY_SIZE];
	int hits;
	int round;
	uint64_t r, ok;

	return 0;
}
