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
#include <linux/i2c.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define AP3216C_SYSTEMCONG 0x00 /* 配置寄存器 */
#define AP3216C_INTSTATUS 0X01 /* 中断状态寄存器 */
#define AP3216C_INTCLEAR 0X02 /* 中断清除寄存器 */
#define AP3216C_IRDATALOW 0x0A /* IR 数据低字节 */
#define AP3216C_IRDATAHIGH 0x0B /* IR 数据高字节 */
#define AP3216C_ALSDATALOW 0x0C /* ALS 数据低字节 */
#define AP3216C_ALSDATAHIGH 0X0D /* ALS 数据高字节 */
#define AP3216C_PSDATALOW 0X0E /* PS 数据低字节 */
#define AP3216C_PSDATAHIGH 0X0F /* PS 数据高字节 */

#define DEVICE_NAME "ap3216_dev"
static int major = 0;
static struct class *demo_class = NULL;
static struct device *demo_device = NULL;
struct i2c_client *ap3216c_client;

unsigned short ir=0;
unsigned short als=0;
unsigned short ps=0;

static s32 ap3216c_write_regs(u8 reg,u8 *buf, u8 len)
{
    u8 b[256];
    struct i2c_msg msg;
    b[0] = reg;
    memcpy(&b[1],buf,len);

    msg.addr = ap3216c_client->addr;
    msg.flags = 0;
    msg.buf = b;
    msg.len =len+1;
    return i2c_transfer(ap3216c_client->adapter,&msg,1);
}

static int ap3216c_read_regs(u8 reg, void *val,int len)
{
    int ret;
    struct i2c_msg msg[2];

    msg[0].addr = ap3216c_client->addr;
    msg[0].flags = 0;
    msg[0].buf = &reg;
    msg[0].len = 1;

    msg[1].addr = ap3216c_client->addr;
    msg[1].flags = I2C_M_RD;
    msg[1].buf = val;
    msg[1].len = len;

    ret = i2c_transfer(ap3216c_client->adapter,msg,2);
    if(ret == 2)
    {
        ret =0;
    }
    else{
        printk("i2c_transfer failed\n");
        ret = -1;
        
    }
    return ret;
}

static unsigned char ap3216c_read_reg(u8 reg)
{
    u8 data = 0;
    ap3216c_read_regs(reg,&data,1);
    return data;
}

static void ap3216c_write_reg(u8 reg, u8 data)
{
    u8 buf = 0;
    buf = data;
    ap3216c_write_regs(reg,&buf,1);
}

void ap3216c_readdata(void)
{
    unsigned char i = 0;
    unsigned char buf[6];
    for(i=0;i<6;i++)
    {
        buf[i] = ap3216c_read_reg(AP3216C_IRDATALOW + i);
        
    }
    if(buf[0]&0x80)
        ir = 0;
    else
        ir = ((unsigned short)buf[1] << 2 )| (buf[0] & 0x03);
    
    als = ((unsigned short)buf[3] << 8 ) | buf[2] ;

    if(buf[4] & 0x40)
        ps = 0;
    else 
        ps = ((unsigned short)(buf[5] & 0x3F) << 4) | (buf[4] & 0x0F);

}

static ssize_t ap3216c_read(struct file *filp, char __user *buf,size_t cnt, loff_t *off)
{
    short data[3];
    ap3216c_readdata();
    data[0] = ir;
    data[1] = als;
    data[2] = ps;
    copy_to_user(buf, data, sizeof(data));
    return 0;
}

static int ap3216c_open(struct inode *inode,struct file *file)
{
    
    return 0;
}

static const struct file_operations demo_fops = {
    .owner = THIS_MODULE,
    .open = ap3216c_open,
    .read = ap3216c_read,
};

static int ap3216c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret;
    char buf;
    printk("matched!\n");
    ap3216c_client = client;
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
	demo_device = device_create(demo_class,NULL,MKDEV(major,0),NULL,"ap3216_demo");
	if(IS_ERR(demo_device)){
		ret = PTR_ERR(demo_device);
		goto device_create_fail;
	}
printk("chipid:%d",ap3216c_read_reg(0x00));


    ap3216c_write_reg(AP3216C_SYSTEMCONG, 0x04);
    mdelay(50);
   ap3216c_write_reg(AP3216C_SYSTEMCONG, 0x03);
    return 0;

device_create_fail:
	class_destroy(demo_class);
class_create_fail:
	unregister_chrdev(major,DEVICE_NAME);
register_chrdev_fail:
	return ret;
}
static int ap3216c_remove(struct i2c_client *client)
{
    unregister_chrdev(major,DEVICE_NAME);
	device_destroy(demo_class,MKDEV(major,0));
	class_destroy(demo_class);
    return 0;
}

//两种匹配方法
static const struct i2c_device_id ap3216c_id[]= {
    {"alientek,ap3216c", 0},
    {}
};
static const struct of_device_id ap3216c_of_match[]= {
    {.compatible = "alientek,ap3216c"},
    {}
};

static struct i2c_driver ap3216c_driver = {
    .probe = ap3216c_probe,
    .remove = ap3216c_remove,
    .driver ={
        .owner = THIS_MODULE,
        .name = "ap3216c",
        .of_match_table = ap3216c_of_match,
    },
    .id_table = ap3216c_id,
};

static int __init demo_module_init(void)
{
	
	return i2c_add_driver(&ap3216c_driver);
}


static void __exit demo_module_exit(void)
{
   i2c_del_driver(&ap3216c_driver);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");