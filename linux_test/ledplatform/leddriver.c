#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/irq.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/platform_device.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define DEVICE_NAME "gpio_led"


struct device_node *nd;
int led_gpio;

static int major = 0;
static struct class *demo_class = NULL;
static struct device *demo_device = NULL;

static int demo_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	return 0;
}

static ssize_t demo_write(struct file *pfile, const char __user *puser, size_t count, loff_t *poffset)
{
	char data;
	if(copy_from_user(&data,puser,1) != 0)
		return -EFAULT;
	if(data == '0')
		{
			gpio_set_value(led_gpio, 0);
		}
	else
		{
			gpio_set_value(led_gpio, 1);
		}	

    printk(KERN_WARNING "%s\n",__FUNCTION__);
	return count;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
.write = demo_write,
};

static int led_probe(struct platform_device *dev)
{
    int ret;
    printk(KERN_WARNING "led driver and device has matched~\n");
    
	major = register_chrdev(major,DEVICE_NAME,&demo_fops);
	if(major < 0)
	{
		ret = major;
		goto register_chrdev_fail;
	}
	demo_class = class_create(THIS_MODULE,DEVICE_NAME);
	if(IS_ERR(demo_class)){
		ret = PTR_ERR(demo_class);
		goto class_create_fail;
	}
	demo_device = device_create(demo_class,NULL,MKDEV(major,0),NULL,"demo_dev");
	if(IS_ERR(demo_device)){
		ret = PTR_ERR(demo_device);
		goto device_create_fail;
	}

     nd = of_find_node_by_path("/led");
    if(nd == NULL)
    {
        ret = -EINVAL;
        goto fail_findnode;
    }
    led_gpio = of_get_named_gpio(nd,"beep-gpio",0);
    if(led_gpio < 0){
        printk("can not find led gpio\r\n");
        ret = -EINVAL;
        goto fail_findnode;
    }
    printk("beep gpio num =%d\r\n",led_gpio);
    //gpio_request(gpioled.led_gpio,"led-gpio");

    gpio_direction_output(led_gpio,0); 

    return 0;

device_create_fail:
	class_destroy(demo_class);
class_create_fail:
	unregister_chrdev(major,DEVICE_NAME);
register_chrdev_fail:
	return ret;
fail_findnode:
    return ret;

}

static int led_remove(struct platform_device *dev)
{
    gpio_set_value(led_gpio, 1);
    printk(KERN_WARNING "%s\n",__FUNCTION__);	
	unregister_chrdev(major,DEVICE_NAME);
	device_destroy(demo_class,MKDEV(major,0));
	class_destroy(demo_class);
    return 0;
}

static const struct of_device_id led_of_match[] = {
    {.compatible = "atkalpha-led"},
    { }
};
static struct platform_driver led_driver = {
    .driver = {
        .name = "imx6ul-led",
        .of_match_table = led_of_match,
    },
    .probe = led_probe,
    .remove = led_remove,
};

static int __init demo_module_init(void)
{
	
	return platform_driver_register(&led_driver);
}


static void __exit demo_module_exit(void)
{
    return platform_driver_unregister(&led_driver);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");

