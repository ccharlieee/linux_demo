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
#include <linux/input.h>
#include <linux/of_irq.h>
#include <linux/irq.h>


#define DEVICE_NAME "gpio_led"


struct device_node *nd;
int key_gpio;
int irqnum;

static struct input_dev *input_key;

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
			//gpio_set_value(led_gpio, 0);
		}
	else
		{
			//gpio_set_value(led_gpio, 1);
		}	

    printk(KERN_WARNING "%s\n",__FUNCTION__);
	return count;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
.write = demo_write,
};

static irqreturn_t key0_handler(int irq, void *dev_id)
{
	int val;
	val = gpio_get_value(key_gpio);
	if(val == 0)
	{
		input_report_key(input_key,1,1);
		input_sync(input_key);
	}
	return IRQ_RETVAL(IRQ_HANDLED);
}

static int led_probe(struct platform_device *dev)
{
    printk(KERN_WARNING "led driver and device has matched~\n");

	input_key = input_allocate_device();
	input_key->name = "demo_key_input";
	input_key->phys = "demo_phy_input";

	input_key->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);
	input_key->keybit[BIT_WORD(KEY_0)] |= BIT_MASK(KEY_0);

	input_register_device(input_key);

    nd = of_find_node_by_path("/key");
    if(nd == NULL)
    {
		printk(KERN_WARNING " nd = null not find \n ");
        return -EINVAL;
    }
    key_gpio = of_get_named_gpio(nd,"key-gpio",0);
    if(key_gpio < 0){
        printk("can not find key gpio\r\n");
        return -EINVAL;
    }
    printk("key gpio num =%d\r\n",key_gpio);
    //gpio_request(gpioled.led_gpio,"led-gpio");

    gpio_direction_input(key_gpio);
	irqnum = irq_of_parse_and_map(nd,0);
	requst_irq(irqnum, key0_handler, IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING,"KEY",input_key);

	
    return 0;
}

static int led_remove(struct platform_device *dev)
{
    input_unregister_device(input_key);

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

