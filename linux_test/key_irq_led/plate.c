#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
 #include <linux/of_address.h>
 #include <linux/of_gpio.h>
 #include <linux/semaphore.h>
 #include <linux/timer.h>
 #include <linux/of_irq.h>
#include <linux/irq.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <linux/types.h>
 #include <linux/kernel.h>
 #include <linux/delay.h>
#include <linux/ide.h>
 #include <linux/init.h>
 #include <linux/module.h>
#include <linux/errno.h>

#define DEVICE_NAME "key_demo"
struct device_node *ndled;
int led_gpio;
struct device_node *nd;
int key_gpio;
struct mutex lock;
struct timer_list timer;
int keyrelaese = 0;
static int major = 0;
static struct class *demo_class = NULL;
static struct device *demo_device = NULL;

int led_status = 0;
void led_switch(void)
{
	if(led_status == 0)
	{
		led_status = 1 ;

	}
	else
	{
		led_status = 0;
	}
}
static irqreturn_t key_handler(int irq, void *dev_id)
{
	printk(KERN_WARNING "in key_handler\n");
	mod_timer(&timer, jiffies + msecs_to_jiffies(20));
	return IRQ_RETVAL(IRQ_HANDLED);
}

void timer_function(unsigned long arg)
{
	char val;
	val = gpio_get_value(key_gpio);
	printk(KERN_WARNING "in timer function\n");
	if(val == 0)
	{
		keyrelaese = 1;
		gpio_set_value(led_gpio, led_status);	 
	}
}
static int demo_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	return 0;
}
static int demo_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
static ssize_t demo_read(struct file *pfile, char __user *puser, size_t count, loff_t *poffset)
{
	int ret = 0;
	
	ret = copy_to_user(puser, &keyrelaese, sizeof(keyrelaese));

    keyrelaese = 0;
	return ret;
}
static ssize_t demo_write(struct file *pfile, const char __user *puser, size_t count, loff_t *poffset)
{
    printk(KERN_WARNING "%s\n",__FUNCTION__);
	return count;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
.read = demo_read,
.write = demo_write,
.release = demo_release,
};

static int __init demo_module_init(void)
{
	int ret = 0;
	int key_irq;
	mutex_init(&lock); 
	printk(KERN_WARNING "%s\n",__FUNCTION__);
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
	demo_device = device_create(demo_class,NULL,MKDEV(major,0),NULL,"key_demo");
	if(IS_ERR(demo_device)){
		ret = PTR_ERR(demo_device);
		goto device_create_fail;
	}
	/*
	ndled = of_find_node_by_path("/led");
    if(ndled == NULL)
    {
        ret = -EINVAL;

    }
    led_gpio = of_get_named_gpio(nd,"beep-gpio",0);
    if(led_gpio < 0){
        printk("can not find led gpio\r\n");
        ret = -EINVAL;
   
    }
    printk("beep gpio num =%d\r\n",led_gpio);
    //gpio_request(gpioled.led_gpio,"led-gpio");

    gpio_direction_output(led_gpio,0); */

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
	gpio_request(key_gpio,"key");
	gpio_direction_input(key_gpio);

	key_irq = gpio_to_irq(key_gpio);
	ret = request_irq(key_irq, key_handler, IRQF_TRIGGER_FALLING , "key_irq", NULL);
	if(ret < 0)
	{
		printk(KERN_WARNING " request_irq failed  ret = %d\n ", ret);
	}

	init_timer(&timer);
	timer.function = timer_function;


return 0;
device_create_fail:
	class_destroy(demo_class);
class_create_fail:
	unregister_chrdev(major,DEVICE_NAME);
register_chrdev_fail:
	return ret;
}
static void __exit demo_module_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);	
	unregister_chrdev(major,DEVICE_NAME);
	device_destroy(demo_class,MKDEV(major,0));
	class_destroy(demo_class);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");

