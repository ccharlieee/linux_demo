#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>

#define LED_MAJOR 200
#define LED_NAME "LED"
/*  led  gpio3
#define CCM_CCGR5_BASE (0X020C406C)
#define SW_MUX_CTL_PAD_SNVS_TAMPER1_base (0X020E0068)
#define SW_PAD_CTL_PAD_SNVS_TAMPER1_base (0X020E02F4)
#define GPIO5_DR_base   (0X0209C000)
#define GPIO5_GDIR_base (0X0209C004)*/
//物理地址  beep
#define CCM_CCGR5_BASE (0X020C407C)
#define SW_MUX_CTL_PAD_SNVS_TAMPER1_base (0X0229000C) 
#define SW_PAD_CTL_PAD_SNVS_TAMPER1_base (0x02290050) 
#define GPIO5_DR_base   (0x020AC000)
#define GPIO5_GDIR_base (0x020AC004)

//虚拟地址
static void __iomem *IMX6U_CCM_CCGR5;
static void __iomem *SW_MUX_CTL_PAD_SNVS_TAMPER1;
static void __iomem *SW_PAD_CTL_PAD_SNVS_TAMPER1;
static void __iomem *GPIO5_DR;
static void __iomem *GPIO5_GDIR;


static int led_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return 0;
}

static ssize_t led_write (struct file *pfile, const char __user *pbuf, size_t count,loff_t *off){
    
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return 0;
}

static int led_release (struct inode *pinode, struct file *pfile){
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return 0;
}
static struct file_operations led_fops={
    .owner = THIS_MODULE,
    .write = led_write,
    .open = led_open,
    .release = led_release,
};
static int __init led_init(void)
{
    int ret = 0;
	int val = 0;
    ret = register_chrdev(LED_MAJOR,LED_NAME,&led_fops);
    if(ret < 0)
    {
        printk("register chrdev failed\n");
        return -EIO;
    }
	//初始化
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
/*	
	val = readl(GPIO5_DR);
	val &=~ (1<<1);
	writel(val,GPIO5_DR);*/
	
	
   printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return 0;
}

static void __exit led_exit(void)
{
	int val;
	val = readl(GPIO5_DR);
	val |= (1<<1);
	writel(val,GPIO5_DR);

	iounmap(IMX6U_CCM_CCGR5);
	iounmap(SW_MUX_CTL_PAD_SNVS_TAMPER1);
	iounmap(SW_PAD_CTL_PAD_SNVS_TAMPER1);
	iounmap(GPIO5_DR);
	iounmap(GPIO5_GDIR);
	
    unregister_chrdev(LED_MAJOR,LED_NAME);

    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ccharliee");
