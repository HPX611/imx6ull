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
#include <linux/of_irq.h> 
#include <linux/irq.h> 
#include <asm/mach/map.h> 
#include <asm/uaccess.h> 
#include <asm/io.h> 
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/fcntl.h>
#include <linux/platform_device.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/miscdevice.h>

#define MISCBEEP_NAME "miscbeep"
#define MISCBEEP_MINOR 144
#define BEEPON 1
#define BEEPOFF 0

struct miscbeep_dev{
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    int beep_gpio;
};

struct miscbeep_dev miscbeep;

static int miscbeep_open(struct inode *inode,struct file *filp)
{
    filp->private_data = &miscbeep;
    return 0;
}

static ssize_t miscbeep_write(struct file *filp,const char __user *buf,size_t cnt,loff_t *offt)
{
    int retvalue;
    unsigned char databuf[1];
    unsigned char beepstat;
    struct miscbeep_dev *dev = filp->private_data;

    retvalue = copy_from_user(databuf,buf,cnt);

    beepstat = databuf[0];
    if(beepstat == BEEPON)
        gpio_set_value(dev->beep_gpio,0);
    else if(beepstat == BEEPOFF)
        gpio_set_value(dev->beep_gpio,1);
    
    return 0;
}

static struct file_operations miscbeep_fops = {
    .owner = THIS_MODULE,
    .open = miscbeep_open,
    .write = miscbeep_write,
};

static struct miscdevice beep_miscdev = {
    .minor = MISCBEEP_MINOR,
    .name = MISCBEEP_NAME,
    .fops = &miscbeep_fops,
};

static int miscbeep_probe(struct platform_device *dev)
{
    int ret = 0;
    printk("beep driver and device was matched\r\n");

    miscbeep.nd = of_find_node_by_path("/beep");

    miscbeep.beep_gpio = of_get_named_gpio(miscbeep.nd,"beep-gpio",0);

    gpio_request(miscbeep.beep_gpio,"beep");
    gpio_direction_output(miscbeep.beep_gpio,1);

    misc_register(&beep_miscdev);

    return 0;
}

static int miscbeep_remove(struct platform_device *dev)
{
    gpio_set_value(miscbeep.beep_gpio,1);
    gpio_free(miscbeep.beep_gpio);
    misc_deregister(&beep_miscdev);
    return 0;
}

static const struct of_device_id beep_of_match[] ={
    {.compatible = "atkalpha-beep", },
    {},
};

static struct platform_driver  beep_driver = {
    .driver = {
        .name = "imx6ul-beep",
        .of_match_table = beep_of_match,
    },
    .probe = miscbeep_probe,
    .remove = miscbeep_remove,
};

static int __init miscbeep_init(void)
{
    return platform_driver_register(&beep_driver);
}

static void __exit miscbeep_exit(void)
{
    platform_driver_unregister(&beep_driver);
}

module_init(miscbeep_init);
module_exit(miscbeep_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hpx");
