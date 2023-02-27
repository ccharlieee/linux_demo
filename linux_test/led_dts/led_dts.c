#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>


#define DTSLED_CNT 1
#define DTSLED_NAME "dtsled"
struct dtsled_dev{
	dev_t devid;
	int major;
	int minor;
	struct cdev cdev;

};
struct dtsled_dev dtsled;

static struct file_operations dtsled_fops={
    .owner = THIS_MODULE,
   // .write = dtsled_write,
   // .open = dtsled_open,
   // .release = dtsled_release,
};

static int __init dtsled_init(void)
{
	int ret = 0 ;
	dtsled.major = 0;
	if(dtsled.major){
		dtsled.devid = MKDEV(dtsled.major,0);
		ret = register_chrdev_region(dtsled.devid,DTSLED_CNT,DTSLED_NAME);
	}
	else{
		ret = alloc_chrdev_region(&dtsled.devid,0,DTSLED_CNT,DTSLED_NAME);
		dtsled.major = MAJOR(dtsled.devid);
		dtsled.minor = MINOR(dtsled.devid);
	}
	if(ret < 0)
	{
		goto fail_devid;
	}
	dtsled.cdev.owner = THIS_MODULE;
	cdev_init(&dtsled.cdev,&dtsled_fops);
	ret = cdev_add(&dtsled.cdev,dtsled.devid,DTSLED_CNT);
	if(ret < 0)
	{
		goto fail_cdev;
	}



	return 0;
fail_cdev:
	unregister_chrdev_region(dtsled.devid,DTSLED_CNT);
fail_devid:
	return ret;
}

static void __exit dtsled_exit(void)
{
	

}
module_init(dtsled_init);
module_exit(dtsled_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccharlieee");
