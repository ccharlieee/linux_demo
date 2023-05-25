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

#define DEVICE_NAME "platform"

static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;


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
    unsigned int val = 0;
	char data;
	if(copy_from_user(&data,puser,1) != 0)
		return -EFAULT;
	if(data == '0')
		{
			val = readl(GPIO1_DR);
			val |= (1<<3);
			writel(val,GPIO1_DR);	
		}
	else
		{
			val = readl(GPIO1_DR);
	        val &=~ (1<<3);
	        writel(val,GPIO1_DR);
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
    int i = 0;
    int ressize[5];
    int val = 0;
    int ret = 0;
    struct resource *ledsource[5];
    printk(KERN_WARNING "led driver and device has matched~\n");

    for(i=0;i<5;i++)
    {
        ledsource[i] = platform_get_resource(dev, IORESOURCE_MEM, i);
        if(!ledsource[i]){
            dev_err(&dev->dev,"no mem resource for always on\n");
            return -ENXIO;
        }
        ressize[i] = resource_size(ledsource[i]);
    }

    IMX6U_CCM_CCGR1 = ioremap(ledsource[0]->start, ressize[0]);
    SW_MUX_GPIO1_IO03 = ioremap(ledsource[1]->start, ressize[1]);
    SW_PAD_GPIO1_IO03 = ioremap(ledsource[2]->start, ressize[2]);
    GPIO1_DR = ioremap(ledsource[3]->start,ressize[3]);
    GPIO1_GDIR = ioremap(ledsource[4]->start, ressize[4]);

    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26);
    val |= (3 << 26); 
    writel(val, IMX6U_CCM_CCGR1);

    writel(5, SW_MUX_GPIO1_IO03);
    writel(0x10B0, SW_PAD_GPIO1_IO03);

    val = readl(GPIO1_GDIR);
    val &= ~(1 << 3); 
    val |= (1 << 3); 
    writel(val, GPIO1_GDIR);

    val = readl(GPIO1_DR);
    val |= (1 << 3) ;
    writel(val, GPIO1_DR);

    
	
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
	demo_device = device_create(demo_class,NULL,MKDEV(major,0),NULL,"demo_dev");
	if(IS_ERR(demo_device)){
		ret = PTR_ERR(demo_device);
		goto device_create_fail;
	}
    return 0;

device_create_fail:
	class_destroy(demo_class);
class_create_fail:
	unregister_chrdev(major,DEVICE_NAME);
register_chrdev_fail:
	return ret;

}

static int led_remove(struct platform_device *dev)
{
    printk(KERN_WARNING "%s\n",__FUNCTION__);	
	unregister_chrdev(major,DEVICE_NAME);
	device_destroy(demo_class,MKDEV(major,0));
	class_destroy(demo_class);

	iounmap(IMX6U_CCM_CCGR1);
	iounmap(SW_MUX_GPIO1_IO03);
	iounmap(SW_PAD_GPIO1_IO03);
	iounmap(GPIO1_DR);
	iounmap(GPIO1_GDIR);
    return 0;
}
static struct platform_driver led_driver = {
    .driver = {
        .name = "imx6ul-led",
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

