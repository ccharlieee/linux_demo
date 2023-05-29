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
#include <linux/input.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>




#define DEVICE_NAME "key_demo"

struct device_node *ndled;

int led_gpio;

struct device_node *nd;

int key_gpio;

struct mutex lock;

struct timer_list timer;

int keyrelaese = 0;

int key_irq;

int led_status = 0;

struct input_dev *input_key;


void led_switch(void)
{
    if(led_status == 0)
    {
        led_status = 1 ;
    }
    else
    {
        led_status = 0;
    }
}
static irqreturn_t key_handler(int irq, void *dev_id)
{

    printk(KERN_WARNING "in key_handler\n");
    mod_timer(&timer, jiffies + msecs_to_jiffies(20));
    return IRQ_RETVAL(IRQ_HANDLED);
}

void timer_function(unsigned long arg)
{
    char val;
    val = gpio_get_value(key_gpio);
    printk(KERN_WARNING "in timer function\n");
    if(val == 0)
    {
        led_switch();
        keyrelaese = 1;

        gpio_set_value(led_gpio, led_status);    

		input_report_key(input_key,KEY_0,1) ;
		input_sync(input_key);

    }	
	else 
	{
		input_report_key(input_key,KEY_0,0) ;
		input_sync(input_key);
	}

}

static int __init demo_module_init(void)

{
    int ret = 0;
    mutex_init(&lock); 

    printk(KERN_WARNING "%s\n",__FUNCTION__);
 
    ndled = of_find_node_by_path("/led");

    if(ndled == NULL)
    {
        ret = -EINVAL;
    }

    led_gpio = of_get_named_gpio(ndled,"beep-gpio",0);

    if(led_gpio < 0){
        printk("can not find led gpio\r\n");
        ret = -EINVAL;
    }

    printk("beep gpio num =%d\r\n",led_gpio);
    gpio_request(led_gpio,"led-gpio");
    gpio_direction_output(led_gpio,0); 
    nd = of_find_node_by_path("/key");

    if(nd == NULL)

    {

        printk(KERN_WARNING " nd = null not find \n ");

        return -EINVAL;

    }
    key_gpio = of_get_named_gpio(nd,"key-gpio",0);
    if(key_gpio < 0){
        printk("can not find key gpio\r\n");
        return -EINVAL;
    }
    printk("key gpio num =%d\r\n",key_gpio);
    gpio_request(key_gpio,"key");
    gpio_direction_input(key_gpio);
    key_irq = gpio_to_irq(key_gpio);
    ret = request_irq(key_irq, key_handler, IRQF_TRIGGER_FALLING| IRQF_TRIGGER_RISING, "key_irq", NULL);
    if(ret < 0)
    {
        printk(KERN_WARNING " request_irq failed  ret = %d\n ", ret);
    }
	input_key = input_allocate_device();
	input_key->name = "input_key";
	__set_bit(EV_KEY, input_key->evbit);
	__set_bit(EV_REP, input_key->evbit);

	__set_bit(KEY_0,input_key->keybit);

	ret = input_register_device(input_key);
	if(ret < 0)
    {
        printk(KERN_WARNING " input_register failed  ret = %d\n ", ret);
    }

    init_timer(&timer);

    timer.function = timer_function;

return 0;

}

static void __exit demo_module_exit(void)

{

    printk(KERN_WARNING "%s\n",__FUNCTION__);    
    del_timer(&timer);
    gpio_free(key_gpio);
    gpio_free(led_gpio);
    free_irq(key_irq,NULL);
	input_unregister_device(input_key);
	input_free_device(input_key);
}



module_init(demo_module_init);

module_exit(demo_module_exit);



MODULE_LICENSE("GPL");

MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");



