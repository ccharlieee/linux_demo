#include <linux/module.h>	/* module_init */
#include <linux/fs.h>	/* file_operations */
#include <linux/device.h>	/* class device */
#include <linux/sched.h>		/* current */
#include <linux/mount.h>		/* struct vfsmount */
#include <linux/platform_device.h>

#define DEVICE_NAME "demo_char"
#define DEMO_PLATFORM_NAME 		"demo_platform_key"

#define CCM_CCGR1_BASE (0X020C406C) 
#define SW_MUX_GPIO1_IO03_BASE (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE (0X020E02F4)
#define GPIO1_DR_BASE (0X0209C000)
#define GPIO1_GDIR_BASE (0X0209C004)
#define REGISTER_LENGTH 4

static struct resource demo_led_resource[] = {
	[0] = {
		.start = CCM_CCGR1_BASE,
		.end =  (CCM_CCGR1_BASE + REGISTER_LENGTH - 1),
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = SW_MUX_GPIO1_IO03_BASE,
		.end = (SW_MUX_GPIO1_IO03_BASE + REGISTER_LENGTH - 1),
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = SW_PAD_GPIO1_IO03_BASE,
		.end = (SW_PAD_GPIO1_IO03_BASE + REGISTER_LENGTH - 1),
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = GPIO1_DR_BASE,
		.end = (GPIO1_DR_BASE + REGISTER_LENGTH - 1),
		.flags = IORESOURCE_MEM,
	},
	[4] = {
		.start = GPIO1_GDIR_BASE,
		.end = (GPIO1_GDIR_BASE + REGISTER_LENGTH - 1),
		.flags = IORESOURCE_MEM,
	}
};

static void demo_release(struct device *dev)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	
}

static struct platform_device device_demo_led = {
	.name = "imx6ul-led",
	.id = -1,
	.num_resources = ARRAY_SIZE(demo_led_resource), 
	.resource = demo_led_resource,
	.dev = {
		.release = &demo_release,
	},
	.num_resources = ARRAY_SIZE(demo_led_resource),
	.resource = demo_led_resource,
};

static int __init demo_module_init(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return platform_device_register(&device_demo_led);
}
static void __exit demo_module_exit(void)
{
	platform_device_unregister(&device_demo_led);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");

