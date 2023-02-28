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
	struct class *class;
	struct device *device;
	
	struct device_node *nd;
};
struct dtsled_dev dtsled;
static void __iomem *IMX6U_CCM_CCGR5;
static void __iomem *SW_MUX_CTL_PAD_SNVS_TAMPER1;
static void __iomem *SW_PAD_CTL_PAD_SNVS_TAMPER1;
static void __iomem *GPIO5_DR;
static void __iomem *GPIO5_GDIR;

#define LEDOFF 0
#define LEDON 1



static int dtsled_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
	


    return 0;
}
static void led_switch(u8 sta)
{
	u32 val =0;
	if(sta == LEDON){
		val =readl(GPIO5_DR);
		val &= ~(1 << 3);
		writel(val,GPIO5_DR);
	}else if(sta == LEDOFF){
		val =readl(GPIO5_DR);
		val |= (1 << 3);
		writel(val,GPIO5_DR);
	}
}

static ssize_t dtsled_write (struct file *pfile, const char __user *pbuf, size_t count,loff_t *off){
    
    
	int ret;
	unsigned char databuf[1];
	ret = copy_from_user(databuf, pbuf ,count);
	if(ret < 0){
		return -EFAULT;
	}
	led_switch(databuf[0]);

    return 0;
}

static int dtsled_release (struct inode *pinode, struct file *pfile){
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
	


    return 0;
}
static struct file_operations dtsled_fops={
    .owner = THIS_MODULE,
    .write = dtsled_write,
    .open = dtsled_open,
    .release = dtsled_release,
};
static int __init dtsled_init(void)
{
	int ret = 0 ;
	const char *str;
	u32 regdata[10];
	int i = 0;
	unsigned int val = 0;
	
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
	dtsled.class = class_create(THIS_MODULE,DTSLED_NAME);
	if(IS_ERR(dtsled.class))
	{
		ret = PTR_ERR(dtsled.class);
		goto fail_class;
	}
	dtsled.device = device_create(dtsled.class,NULL,dtsled.devid,NULL,DTSLED_NAME);
	if(IS_ERR(dtsled.device))
	{
		ret = PTR_ERR(dtsled.device);
		goto fail_device;
	}
	//获取设备树属性
	dtsled.nd = of_find_node_by_path("/alphaled");
	if(dtsled.nd == NULL)
	{
		ret = -EINVAL;
		goto fail_findnd;
	}
	ret = of_property_read_string(dtsled.nd,"status",&str);
	if(ret < 0)
	{
		ret = -EINVAL;
		goto fail_rs;
	}
	else{
		printk("status = %s\r\n",str);
	}
	ret = of_property_read_u32_array(dtsled.nd,"reg",regdata,10);
	if(ret < 0){
		goto fail_rs;
	}
	else{
		printk("reg data:\r\n");
		for ( i = 0; i < 10; i++)
		{
			printk("%#X  ", regdata[i]);
		}
		printk("\r\n");
		
	}

	IMX6U_CCM_CCGR5 = ioremap(regdata[0],regdata[1]);//一个寄存器32位 4个字节
	SW_MUX_CTL_PAD_SNVS_TAMPER1 = ioremap(regdata[2],regdata[3]);
	SW_PAD_CTL_PAD_SNVS_TAMPER1 = ioremap(regdata[4],regdata[5]);
	GPIO5_DR = ioremap(regdata[6],regdata[7]);
	GPIO5_GDIR = ioremap(regdata[8],regdata[9]);

	val = readl(IMX6U_CCM_CCGR5);
	val &= ~(3<<26);
	val |= 3<<26;
	writel(val,IMX6U_CCM_CCGR5);

	val = readl(GPIO5_GDIR);
	val |= 1 << 3;
	writel(0X5,GPIO5_GDIR);
	
	//初始化
  	writel(0x5,SW_MUX_CTL_PAD_SNVS_TAMPER1);
	writel(0x10B0,SW_PAD_CTL_PAD_SNVS_TAMPER1);

	
	val = readl(GPIO5_DR);
	val &=~ (1<<3);
	writel(val,GPIO5_DR);





	return 0;
fail_rs:

fail_findnd:
	device_destroy(dtsled.class,dtsled.devid);
fail_device:
	class_destroy(dtsled.class);
fail_class:
	cdev_del(&dtsled.cdev);
fail_cdev:
	unregister_chrdev_region(dtsled.devid,DTSLED_CNT);
fail_devid:
	return ret;
}

static void __exit dtsled_exit(void)
{
	iounmap(IMX6U_CCM_CCGR5);
	iounmap(SW_MUX_CTL_PAD_SNVS_TAMPER1);
	iounmap(SW_PAD_CTL_PAD_SNVS_TAMPER1);
	iounmap(GPIO5_DR);
	iounmap(GPIO5_GDIR);
	cdev_del(&dtsled.cdev);
	unregister_chrdev_region(dtsled.devid,DTSLED_CNT);
	device_destroy(dtsled.class,dtsled.devid);
	class_destroy(dtsled.class);
	

}
module_init(dtsled_init);
module_exit(dtsled_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccharlieee");
