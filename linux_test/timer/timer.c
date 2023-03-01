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
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define TIMER_CNT 1
#define TIMER_NAME "timer"

#define CLOSE_CMD  _IO(0XEF, 1)
#define OPEN_CMD   _IO(0XEF, 2)
#define SETPERIOD_CMD  _IOW(0XEF ,3 ,int )

struct timer_dev{
    dev_t devid;
    int major;
    int minor;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    struct timer_list timer;
    int led_gpio;
    int timeperiod;

};
struct timer_dev timer;

static int timer_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);	
    return 0;
}

static ssize_t timer_write (struct file *pfile, const char __user *pbuf, size_t count,loff_t *off){
    

     
    return 0;
}

static int timer_release (struct inode *pinode, struct file *pfile){
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);

    return 0;
}
/*
static int timer_ioctl(struct file *pfile,unsigned int cmd,unsigned long reg)
{
    struct timer_dev *dev =  (struct timer_dev *)filp->private_data;
    switch (cmd)
    {
    case CLOSE_CMD:
        del_timer_sync(&dev->timer);
        break;
    case OPEN_CMD:
        mod_timer(&dev->timer,jiffies + msecs_to_jiffies(dev->timeperiod));
        break;
    case SETPERIOD_CMD:
       
        break;
    }
}*/

static const struct file_operations timer_fops={
    .owner = THIS_MODULE,
    .open = timer_open,
    .write = timer_write,
  //  .unlocked_ioctl = timer_ioctl,
    .release = timer_release,

};


static void timer_func(unsigned long arg)
{
    static int sta = 1;
    struct timer_dev *dev =(struct timer_dev*)arg;
    sta = !sta;
    gpio_set_value(dev->led_gpio,sta);

    mod_timer(&dev->timer,jiffies + msecs_to_jiffies(500));

}
static int led_init(struct timer_dev *dev)
{
    int ret = 0;
    dev->nd = of_find_node_by_path("/gpioled");
    if(dev->nd ==NULL)
    {
        ret = -EINVAL;
        goto fail_fd;
    }
    dev->led_gpio = of_get_named_gpio(dev->nd,"led-gpios",0);
    gpio_request(dev->led_gpio,"led");
    gpio_direction_output(dev->led_gpio,1);


fail_fd:
    return ret;
}
static int __init timer_init(void)
{
    timer.major = 0;
    if(timer.major){
        timer.devid = MKDEV(timer.major,0);
        register_chrdev_region(timer.devid,TIMER_CNT,TIMER_NAME);
    }
    else{
        alloc_chrdev_region(&timer.devid,0,TIMER_CNT,TIMER_NAME);//0 从零开始
        timer.major = MAJOR(timer.devid);
        timer.minor = MINOR(timer.devid);
    }

    timer.cdev.owner = THIS_MODULE;
    cdev_init(&timer.cdev,&timer_fops);
    cdev_add(&timer.cdev,timer.devid,TIMER_CNT);

    timer.class = class_create(THIS_MODULE,TIMER_NAME);
    timer.device = device_create(timer.class,NULL,timer.devid,NULL,TIMER_NAME);
    if(IS_ERR(timer.device)){
        return PTR_ERR(timer.device);
    }

    led_init(&timer);
    init_timer(&timer.timer);
    timer.timeperiod = 500;
    timer.timer.function = timer_func;
    timer.timer.expires = jiffies + msecs_to_jiffies(500);
    timer.timer.data = (unsigned long)&timer;
    add_timer(&timer.timer);


    return 0;

}


static void __exit timer_exit(void)
{
    gpio_set_value(timer.led_gpio,1);
    del_timer(&timer.timer);
    cdev_del(&timer.cdev);
    unregister_chrdev_region(timer.devid,TIMER_CNT);
    device_destroy(timer.class,timer.devid);
    class_destroy(timer.class);

    gpio_free(timer.led_gpio);
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccharlieee");