#ifndef COMMON_H
#define COMMON_H

#define STRIDE (4096)
#define PHYS_OFFSET (0x40000000ULL)
#define PAGE_OFFSET (0xffffff8000000000ULL)
uint64_t virt_to_physmap(uint64_t virtual_address);
#endif
