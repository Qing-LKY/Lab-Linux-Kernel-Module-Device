#include <linux/module.h>
#include <linux/kernel.h>

static int init_hello(void) {
    printk(KERN_INFO "Hello World!");
    return 0;
}

static void exit_hello(void) {
    printk(KERN_INFO "Goodbye World!");
}

MODULE_LICENSE("GPL");
module_init(init_hello);
module_exit(exit_hello);