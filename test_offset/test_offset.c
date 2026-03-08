#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/memory.h> // PAGE_OFFSET が定義されているヘッダー

static int __init test_init(void) {
    printk(KERN_INFO "PAGE_OFFSET: 0x%lx\n", PAGE_OFFSET);
    printk(KERN_INFO "PHYS_OFFSET: 0x%lx\n", memstart_addr);
    return 0;
}
static void __exit test_exit(void) {}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
