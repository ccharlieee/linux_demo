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

#define TIMER_CNT 1
#define TIMER_NAME "timer"

struct timer_dev{
    dev_t devid;
    int major;
    int minor;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    int time;

};
struct timer_dev timer;

static int timer_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);	
    return 0;
}

static ssize_t timer_write (struct file *pfile, const char __user *pbuf, size_t count,loff_t *off){
    
	int ret;

     
    return 0;
}

static int timer_release (struct inode *pinode, struct file *pfile){
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);

    return 0;
}
static const struct file_operations timer_fops={
    .owner = THIS_MODULE,
    .open = timer_open,
    .write = timer_write,
    .release = timer_release,
};

static int __init timer_init(void)
{
    int ret = 0;
    timer.major = 0;
    if(timer.major){
        timer.devid = MKDEV(timer.major,0);
        register_chrdev_region(timer.devid,timer_CNT,timer_NAME);
    }
    else{
        alloc_chrdev_region(&timer.devid,0,timer_CNT,timer_NAME);//0 从零开始
        timer.major = MAJOR(timer.devid);
        timer.minor = MINOR(timer.devid);
    }

    timer.cdev.owner = THIS_MODULE;
    cdev_init(&timer.cdev,&timer_fops);
    cdev_add(&timer.cdev,timer.devid,timer_CNT);

    timer.class = class_create(THIS_MODULE,timer_NAME);
    timer.device = device_create(timer.class,NULL,timer.devid,NULL,timer_NAME);
    if(IS_ERR(timer.device)){
        return PTR_ERR(timer.device);
    }

    return 0;
fail_findnode:
    return ret;
}


static void __exit timer_exit(void)
{
    gpio_set_value(timer.time,1);
    cdev_del(&timer.cdev);
    unregister_chrdev_region(timer.devid,timer_CNT);
    device_destroy(timer.class,timer.devid);
    class_destroy(timer.class);
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccharlieee");