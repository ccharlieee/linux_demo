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
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define GPIOLED_CNT 1
#define GPIOLED_NAME "gpio_led"
#define LEDON 1
#define LEDOFF 0 
struct gpioled_dev{
    dev_t devid;
    int major;
    int minor;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    int led_gpio;

};
struct gpioled_dev gpioled;

static int gpioled_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
	


    return 0;
}

static ssize_t gpioled_write (struct file *pfile, const char __user *pbuf, size_t count,loff_t *off){
    
    
	int ret;
	unsigned char databuf[1];
	ret = copy_from_user(databuf, pbuf ,count);
	if(ret < 0){
		return -EFAULT;
	}
    if(databuf[0] == LEDON){
        gpio_set_value(gpioled.led_gpio,0);
    }
    else{
        gpio_set_value(gpioled.led_gpio,1);
    }
    
	 

    return 0;
}

static int gpioled_release (struct inode *pinode, struct file *pfile){
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);

    return 0;
}
static const struct file_operations gpioled_fops={
    .owner = THIS_MODULE,
    .open = gpioled_open,
    .write = gpioled_write,
    .release = gpioled_release,
};

static int __init led_init(void)
{
    int ret = 0;
    gpioled.major = 0;
    if(gpioled.major){
        gpioled.devid = MKDEV(gpioled.major,0);
        register_chrdev_region(gpioled.devid,GPIOLED_CNT,GPIOLED_NAME);
    }
    else{
        alloc_chrdev_region(&gpioled.devid,0,GPIOLED_CNT,GPIOLED_NAME);//0 从零开始
        gpioled.major = MAJOR(gpioled.devid);
        gpioled.minor = MINOR(gpioled.devid);
    }

    gpioled.cdev.owner = THIS_MODULE;
    cdev_init(&gpioled.cdev,&gpioled_fops);
    cdev_add(&gpioled.cdev,gpioled.devid,GPIOLED_CNT);

    gpioled.class = class_create(THIS_MODULE,GPIOLED_NAME);
    gpioled.device = device_create(gpioled.class,NULL,gpioled.devid,NULL,GPIOLED_NAME);
    if(IS_ERR(gpioled.device)){
        return PTR_ERR(gpioled.device);
    }

    //获取设备节点
    gpioled.nd = of_find_node_by_path("/gpioled");
    if(gpioled.nd == NULL)
    {
        ret = -EINVAL;
        goto fail_findnode;
    }
    gpioled.led_gpio = of_get_named_gpio(gpioled.nd,"led-gpios",0);
    if(gpioled.led_gpio < 0){
        printk("can not find led gpio\r\n");
        ret = -EINVAL;
        goto fail_findnode;
    }
    printk("led gpio num =%d\r\n",gpioled.led_gpio);
    gpio_request(gpioled.led_gpio,"led-gpio");

    gpio_direction_output(gpioled.led_gpio,1); 
    gpio_set_value(gpioled.led_gpio,0);


    return 0;
fail_findnode:
    return ret;
}


static void __exit led_exit(void)
{
    gpio_set_value(gpioled.led_gpio,1);
    cdev_del(&gpioled.cdev);
    unregister_chrdev_region(gpioled.devid,GPIOLED_CNT);
    device_destroy(gpioled.class,gpioled.devid);
    class_destroy(gpioled.class);

}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccharlieee");