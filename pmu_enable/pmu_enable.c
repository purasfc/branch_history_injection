#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/smp.h>

static void enable_pmu(void *info) {
	asm volatile("msr pmuserenr_el0, %0" :: "r"(0xFUL));
	asm volatile("msr pmcntenset_el0, %0" :: "r"(1UL << 31));
	asm volatile("msr pmcr_el0, %0" :: "r"(0x7UL));
}

static void disable_pmu(void *info) {
	asm volatile("msr pmuserenr_el0, %0" :: "r"(0UL));
	asm volatile("msr pmcntenset_el0, %0" :: "r"(0UL));
}

static int __init pmu_init(void) {
	on_each_cpu(enable_pmu, NULL, 1);
	pr_info("pmu_enable: pmccntr_el0 enabled on all CPUs\n");
	return 0;
}

static void __exit pmu_exit(void) {
	on_each_cpu(disable_pmu, NULL, 1);
	pr_info("pmu_enable: pmccntr_el0 disabled\n");
}

module_init(pmu_init);
module_exit(pmu_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Enable userspace PMU access (pmccntr_el0)");

