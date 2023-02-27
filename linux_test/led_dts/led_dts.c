#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>

#define DTSLED_CNT 1
#define DTSLED_NAME "dtsled"
struct dtsled_dev{
	dev_t devid;
	int major;
	int minor;

};
struct stsled_dev dtsled;

static int __init dtsled_init(void)
{
	int ret = 0 ;
	dtsled.major = 0;
	if(dtsled.major){
		dtsled.dev = MKDEV(led.major,0);
		ret = register_chrdev_region(dtsled.devid,DTSLED_CNT,DTSLED_NAME);
	}
	else{
		ret = alloc_chrdev_region(&dtsled.dev,0,DTSLED_CNT,DTSLED_NAME);
		dtsled.major = MAJOR(dtsled.devid);
		dtsled.minor = MINOR(dtsled.devid);
	}
	if(ret < 0)

	return ret;
}

static void __exit dtsled_exit()
{
	

}
moudle_init();
module_exit();
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccharlieee");
