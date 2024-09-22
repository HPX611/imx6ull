#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>


#define BEEP_CNT 1
#define BEEP_NAME "beep"
#define BEEP_OFF 0
#define BEEP_ON 1

struct beep_dev{
    dev_t devid;
    int major,minor;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    int beep_gpio;
};

struct beep_dev beep;

static int beep_open(struct inode *inode,struct file *filp){
    filp->private_data = &beep;
    return 0;
}

static ssize_t beep_write(struct file *filp,const char __user *buf,size_t cnt,loff_t *offt)
{
    int ret = 0;
    u8 databuf[1];
    u8 beepstat;
    struct beep_dev *dev = filp->private_data;

    ret = copy_from_user(databuf,buf,cnt);
    if(ret < 0){
        printk("write error\r\n");
        return -EFAULT;
    }

    beepstat = databuf[0];

    if(beepstat == BEEP_ON){
        gpio_set_value(dev->beep_gpio,0);
    } else if(beepstat == BEEP_OFF){
        gpio_set_value(dev->beep_gpio,1);
    }

    return 0;
}

static int beep_release(struct inode  *inode,struct file *filp)
{
    return 0;
}

static struct file_operations beep_fops = {
    .owner = THIS_MODULE,
    .open = beep_open,
    .write = beep_write,
    .release = beep_release,
};

static int __init beep_init(void){

    //设置BEEP用的GPIO
    //1、获取设备节点
    beep.nd = of_find_node_by_path("/beep");
    if(beep.nd == NULL) {
        printk("node not found\r\n");
        return -EINVAL;
    }

    //2、获取GPIO引脚标号
    beep.beep_gpio = of_get_named_gpio(beep.nd,"beep-gpio",0);
    printk("beep gpio : %d\r\n",beep.beep_gpio);

    //3、设置为输出，输出高电平，默认关闭
    gpio_direction_output(beep.beep_gpio,1);

    //注册设备驱动
    if(beep.major){
        beep.devid = MKDEV(beep.major,0);
        register_chrdev_region(beep.devid,BEEP_CNT,BEEP_NAME);
    } else {
        alloc_chrdev_region(&beep.devid,0,BEEP_CNT,BEEP_NAME);
        beep.major = MAJOR(beep.devid);
        beep.minor = MINOR(beep.devid);
    }

    beep.cdev.owner = THIS_MODULE;
    cdev_init(&beep.cdev,&beep_fops);

    cdev_add(&beep.cdev,beep.devid,BEEP_CNT);

    beep.class = class_create(THIS_MODULE,BEEP_NAME);
    if(IS_ERR(beep.class)){
        return PTR_ERR(beep.class);
    }

    beep.device = device_create(beep.class,NULL,beep.devid,NULL,BEEP_NAME);
    if(IS_ERR(beep.device))
        return PTR_ERR(beep.device);
    
    return 0;

}

static void __exit beep_exit(void)
{
    device_destroy(beep.class,beep.devid);
    class_destroy(beep.class);

    cdev_del(&beep.cdev);
    unregister_chrdev_region(beep.devid,BEEP_CNT);
}

module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hpx");