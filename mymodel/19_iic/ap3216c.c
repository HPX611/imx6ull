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
#include "ap3216creg.h" 

#define AP3216C_CNT 1
#define AP3216C_NAME "ap3216c"

struct ap3216c_dev {
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    struct device_node *nd;
    int major;
    void *private_data; //私有数据 要存放 i2c_client
    unsigned short ir,als,ps;   //三个光传感器数据
};

static struct ap3216c_dev ap3216cdev;

//从ap3216c读多个寄存器的数据
static int ap3216c_read_regs(struct ap3216c_dev *dev,u8 reg,void *val,int len)
{
    int ret =0 ;
    struct i2c_msg msg[2];
    struct i2c_client *client = (struct i2c_client *)dev->private_data;

    //开始创建msg
    //msg[0]是给设备法要读的数据的首地址
    msg[0].addr = client->addr;  //ap3216c地址
    msg[0].flags = 0;           //标记为发送数据
    msg[0].buf = &reg;          //数据首地址
    msg[0].len = 1;             //buf的长度

    //msg[1]是读到的数据的存放位置
    msg[1].addr = client->addr;
    msg[1].flags = I2C_M_RD;
    msg[1].buf = val;   //读取数据缓冲区
    msg[1].len = len;   //读取数据的长度


    ret = i2c_transfer(client->adapter,msg,2);
    if(ret == 2)
        return 0;
    else {
        printk("i2c read failed=%d reg=%06x len=%d\r\n",ret,reg,len);
        return -EREMOTEIO;
    }
    return ret;
}

//向ap3216c写多个寄存器的数据
static s32 ap3216c_write_regs(struct ap3216c_dev *dev,u8 reg, u8 *buf,u8 len)
{
    u8 b[256];
    struct i2c_msg msg;
    struct i2c_client *client = (struct i2c_client *)dev->private_data;

    b[0] = reg;
    memcpy(&b[1],buf,len);

    msg.addr = client->addr;
    msg.flags = 0;
    msg.buf = b;
    msg.len = len + 1;

    return i2c_transfer(client->adapter,&msg,1);
}

//读取指定寄存器的值，读取一个寄存器
static unsigned char ap3216c_read_reg(struct ap3216c_dev *dev,u8 reg)
{
    u8 data = 0;
    ap3216c_read_regs(dev,reg,&data,1);
    return data;
}

//向一个指定的寄存器写数据
static void ap3216c_write_reg(struct ap3216c_dev *dev,u8 reg,u8 data)
{
    u8 buf = 0;
    buf = data;
    ap3216c_write_regs(dev,reg,&buf,1);
}

//读取ap3216c的数据 ALS PS IR 
void ap3216c_readdata(struct ap3216c_dev *dev)
{
    unsigned char i =0 ;
    unsigned char buf[6];

    //读取ALS PS IR 寄存器数据
    for(i=0;i<6;i++)
    {
        buf[i] = ap3216c_read_reg(dev,AP3216C_IRDATALOW + i);
    }

    if(buf[0] & 0x80)
        dev->ir = 0;
    else 
    {
        dev->ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0x03);
    } 

    dev->als = ((unsigned short)buf[3] << 8) | buf[2];

    if(buf[4] & 0x40)
        dev->ps = 0;
    else 
    {
        dev->ps = ((unsigned short)(buf[5] & 0x3f) << 4) | (buf[4] & 0x0f);
    }
}

static int ap3216c_open(struct inode *inode,struct file *filp)
{
    filp->private_data = &ap3216cdev;

    //初始化ap3216c
    ap3216c_write_reg(&ap3216cdev,AP3216C_SYSTEMCONG,0X04);
    mdelay(50);
    ap3216c_write_reg(&ap3216cdev,AP3216C_SYSTEMCONG,0X03);
    return 0;
}

static ssize_t ap3216c_read(struct file *filp,char __user *buf,size_t cnt,loff_t *off)
{
    short data[3];
    long err = 0;
    struct ap3216c_dev *dev = (struct ap3216c_dev*)filp->private_data;

    ap3216c_readdata(dev);

    data[0] = dev->ir;
    data[1] = dev->als;
    data[2] = dev->ps;
    err = copy_to_user(buf,data,sizeof(data));
    return 0;
}

static int ap3216c_release(struct inode *inode,struct file *filp)
{
    return 0;
}

static const struct file_operations ap3216c_ops = {
    .owner = THIS_MODULE,
    .open = ap3216c_open,
    .read = ap3216c_read,
    .release = ap3216c_release,
};

static int ap3216c_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
    //创建设备号
    if(ap3216cdev.major)
    {
        ap3216cdev.devid = MKDEV(ap3216cdev.major,0);
        register_chrdev_region(ap3216cdev.devid,AP3216C_CNT,AP3216C_NAME);
    } else {
        alloc_chrdev_region(&ap3216cdev.devid,0,AP3216C_CNT,AP3216C_NAME);
        ap3216cdev.major = MAJOR(ap3216cdev.devid);
    }

    cdev_init(&ap3216cdev.cdev,&ap3216c_ops);
    cdev_add(&ap3216cdev.cdev,ap3216cdev.devid,AP3216C_CNT);

    ap3216cdev.class = class_create(THIS_MODULE,AP3216C_NAME);

    ap3216cdev.device = device_create(ap3216cdev.class,NULL,ap3216cdev.devid,NULL,AP3216C_NAME);

    ap3216cdev.private_data = client;
    return 0;
}

static int ap3216c_remove(struct i2c_client *client)
{
    cdev_del(&ap3216cdev.cdev);
    unregister_chrdev_region(ap3216cdev.devid,AP3216C_CNT);

    device_destroy(ap3216cdev.class,ap3216cdev.devid);
    class_destroy(ap3216cdev.class);
    return 0;
}

static const struct i2c_device_id ap3216c_id[] = {
    {"alientek,ap3216c",0},
    {}
};

static const struct of_device_id ap3216c_of_match[]={
    {.compatible = "alientek,ap3216c"},
    {}
};

static struct i2c_driver ap3216c_driver = {
    .probe = ap3216c_probe,
    .remove = ap3216c_remove,
    .driver = {
        .owner = THIS_MODULE,
        .name = "ap3216c",
        .of_match_table = ap3216c_of_match,
    },
    .id_table = ap3216c_id,
};

static int __init ap3216c_init(void)
{
    int ret = 0;
    ret = i2c_add_driver(&ap3216c_driver);
    return 0;
}

static void __exit ap3216c_exit(void)
{
    i2c_del_driver(&ap3216c_driver);
}

module_init(ap3216c_init);
module_exit(ap3216c_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hpx");
