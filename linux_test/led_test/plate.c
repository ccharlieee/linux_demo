#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/device.h>	/* class device */
#include <linux/sched.h>		/* current */
#include <linux/mount.h>		/* struct vfsmount */

#define DEVICE_NAME "led_test"
//物理地址  beep
#define CCM_CCGR5_BASE (0X020C407C)
#define SW_MUX_CTL_PAD_SNVS_TAMPER1_base (0X0229000C)
#define SW_PAD_CTL_PAD_SNVS_TAMPER1_base (0x02290050)
#define GPIO5_DR_base   (0x020AC000)
#define GPIO5_GDIR_base (0x020AC004)
/*  led  gpio3
#define CCM_CCGR5_BASE (0X020C406C)
#define SW_MUX_CTL_PAD_SNVS_TAMPER1_base (0X020E0068)
#define SW_PAD_CTL_PAD_SNVS_TAMPER1_base (0X020E02F4)
#define GPIO5_DR_base   (0X0209C000)
#define GPIO5_GDIR_base (0X0209C004)*/
//虚拟地址
static void __iomem *IMX6U_CCM_CCGR5;
static void __iomem *SW_MUX_CTL_PAD_SNVS_TAMPER1;
static void __iomem *SW_PAD_CTL_PAD_SNVS_TAMPER1;
static void __iomem *GPIO5_DR;
static void __iomem *GPIO5_GDIR;

static int major = 0;
static struct class *demo_class = NULL;
static struct device *demo_device = NULL;

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
    printk(KERN_WARNING "%s\n",__FUNCTION__);
	return count;
}
static ssize_t demo_write(struct file *pfile, const char __user *puser, size_t count, loff_t *poffset)
{
	unsigned int val = 0;
	char data;
	if(copy_from_user(&data,puser,1) != 0)
		return -EFAULT;
	if(data == '0')
		{
			val = readl(GPIO5_DR);
			val |= (1<<1);
			writel(val,GPIO5_DR);	
		}
	else
		{
			val = readl(GPIO5_DR);
	        val &=~ (1<<1);
	        writel(val,GPIO5_DR);
		}	

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
	int val = 0;
	
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

	IMX6U_CCM_CCGR5 = ioremap(CCM_CCGR5_BASE,4);//一个寄存器32位 4个字节
	SW_MUX_CTL_PAD_SNVS_TAMPER1 = ioremap(SW_MUX_CTL_PAD_SNVS_TAMPER1_base,4);
	SW_PAD_CTL_PAD_SNVS_TAMPER1 = ioremap(SW_PAD_CTL_PAD_SNVS_TAMPER1_base,4);
	GPIO5_DR = ioremap(GPIO5_DR_base,4);
	GPIO5_GDIR = ioremap(GPIO5_GDIR_base,4);
	
	val = readl(IMX6U_CCM_CCGR5);
	val &= ~(3<<20);
	val |= 3<<20;
	writel(val,IMX6U_CCM_CCGR5);
//	writel(val,GPIO5_GDIR);
	writel(0X2,GPIO5_GDIR);

	
	//初始化
  	writel(0x5,SW_MUX_CTL_PAD_SNVS_TAMPER1);
	writel(0x10B0,SW_PAD_CTL_PAD_SNVS_TAMPER1);

//	val = readl(GPIO5_GDIR);
//	val |= 1<<1;
//	writel(val,GPIO5_GDIR);
	
	val = readl(GPIO5_DR);
	val &=~ (1<<1);
	writel(val,GPIO5_DR);

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

	iounmap(IMX6U_CCM_CCGR5);
	iounmap(SW_MUX_CTL_PAD_SNVS_TAMPER1);
	iounmap(SW_PAD_CTL_PAD_SNVS_TAMPER1);
	iounmap(GPIO5_DR);
	iounmap(GPIO5_GDIR);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");
