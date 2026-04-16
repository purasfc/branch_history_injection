#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include "targets.h"
#include "helper.h"

#define BHI_HIT_SYSCALL 441
#define BHI_ENABLE_SYSCALL 442
#define BHI_ENABLE_SYSCALL 172
#define ITERS 1000
