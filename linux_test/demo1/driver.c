#include <linux/module.h>

static int __init demo_module_init(void)
{
    printk("demo_init ok...\n");
    return 0;
}

static int __exit demo_module_exit(void)
{
  //  printk("demo_exit ok...\n");
}

module_init(demo_module_init);
module_exit(demo_module_exit);
MODULE_LICENSE("GPL");