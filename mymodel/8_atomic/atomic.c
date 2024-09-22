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

#define GPIOLED_CNT 1               //设备号个数
#define GPIOLED_NAME "gpioled"  //设备名字
#define LEDOFF 0    //关灯
#define LEDON 1     //开灯

struct gpioled_dev{
    dev_t devid;                //设备号
    struct cdev cdev;           //cdev
    struct class *class;        //类
    struct device *device;      //设备
    int major;                  //主设备号
    int minor;                  //次设备号
    struct device_node *nd;     //设备节点
    int led_gpio;               //gpio的编号
    atomic_t lock;
};

struct gpioled_dev gpioled;

static int led_open(struct inode *inode,struct file *filp){

    if(!atomic_dec_and_test(&gpioled.lock)){
        atomic_inc(&gpioled.lock);
        return -EBUSY;
    }

    filp->private_data = &gpioled;
    return 0;
}

static ssize_t led_read(struct file *filp,char __user *buf,size_t cnt,loff_t *offt){
    return 0;
}

static ssize_t led_write(struct file *filp,const char __user *buf,size_t cnt,loff_t *offt){
    int retvalue = 0;
    u8 databuf[1];
    u8 ledstat;
    struct gpioled_dev *dev = filp->private_data;

    retvalue = copy_from_user(databuf,buf,cnt);
    if(retvalue < 0){
        printk("kernel write failed!\r\n");
        return -EFAULT;
    }

    ledstat = databuf[0];

    if(ledstat == LEDON){
        gpio_set_value(dev->led_gpio,0);
    } else if(ledstat == LEDOFF){
        gpio_set_value(dev->led_gpio,1);
    }

    return 0;
}

static int led_release(struct inode *inode,struct file *filp){

    struct gpioled_dev *dev = filp->private_data;
    atomic_inc(&dev->lock);
    return 0;
}

static struct file_operations gpioled_fops = {
    .owner =  THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release = led_release,
};

static int __init led_init (void){
    int ret = 0;


    atomic_set(&gpioled.lock,1);
    //先设置LED使用的GPIO
    //1、获取设备节点
    gpioled.nd = of_find_node_by_path("/gpioled");
    if(gpioled.nd == NULL){
        printk("find node failed\r\n");
        ret = -EINVAL;
        goto fail_findnode;
    }

    //2、获得GPIO属性和编号
    gpioled.led_gpio = of_get_named_gpio(gpioled.nd,"led-gpio",0);
    if(gpioled.led_gpio < 0){
        printk("failed get gpio id\r\n");
        ret = -EINVAL;
        goto fail_getgpioid;
    }

    printk("led-gpio num = %d \r\n",gpioled.led_gpio);

    //3、设置GPIO03为输出的并且输出高电平，默认关闭LED灯
    ret = gpio_direction_output(gpioled.led_gpio,1);
    if(ret < 0){
        printk("failed set gpio\r\n");
    }

    //注册字符设备驱动
    //1、创建设备号
    if(gpioled.major){
        gpioled.devid = MKDEV(gpioled.major,0);
        ret = register_chrdev_region(gpioled.devid,GPIOLED_CNT,GPIOLED_NAME);
        if(ret < 0){
            printk("failed to register\r\n");
            goto fail_register;
        }
        gpioled.minor = MINOR(gpioled.devid);
    } else {
        alloc_chrdev_region(&gpioled.devid,0,GPIOLED_CNT,GPIOLED_NAME);
        gpioled.major = MAJOR(gpioled.devid);
        gpioled.minor = MINOR(gpioled.devid);
    }

    printk("gpioled major = %d minor = %d\r\n",gpioled.major,gpioled.minor);

    //2、初始化cdev
    gpioled.cdev.owner = THIS_MODULE;
    cdev_init(&gpioled.cdev,&gpioled_fops);

    //3、添加一个cdev
    cdev_add(&gpioled.cdev,gpioled.devid,GPIOLED_CNT);

    //4、创建类
    gpioled.class = class_create(THIS_MODULE,GPIOLED_NAME);
    if(IS_ERR(gpioled.class)){
        return PTR_ERR(gpioled.class);
    }

    //5、创建设备
    gpioled.device = device_create(gpioled.class,NULL,gpioled.devid,NULL,GPIOLED_NAME);
    if(IS_ERR(gpioled.device)){
        return PTR_ERR(gpioled.device);
    }

    return 0;



fail_register:
fail_getgpioid:
fail_findnode:
    return ret;
}

static void __exit led_exit(void){
    device_destroy(gpioled.class,gpioled.devid);
    class_destroy(gpioled.class);

    cdev_del(&gpioled.cdev);
    unregister_chrdev_region(gpioled.devid,GPIOLED_CNT);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hpx");