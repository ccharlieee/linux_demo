#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
struct class *demo_class;

static int demo_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return 0;
}
static ssize_t demo_read (struct file *pfile, char __user *pbuf, size_t count, loff_t *off)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return 0;
}
static ssize_t demo_write (struct file *pfile, const char __user *pbuf, size_t count,loff_t *off){
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return count;
}

static int demo_release (struct inode *pinode, struct file *pfile){
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    return 0;
}
static struct file_operations fops ={
  .open = demo_open,
  .read = demo_read,
  .write = demo_write,
  .release = demo_release,
};
static int __init demo_module_init(void)
{
    printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);
    register_chrdev(242,"demo_char",&fops);
    demo_class = class_create(THIS_MODULE,"demo_class");


    return 0;
}

static void __exit demo_module_exit(void)
{
  printk(KERN_WARNING"L%d->%s()\n",__LINE__,__FUNCTION__);

  class_destroy(demo_class);
  unregister_chrdev(242,"demo_char");

}

module_init(demo_module_init);
module_exit(demo_module_exit);
MODULE_LICENSE("GPL");