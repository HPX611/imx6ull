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

#define DTSLED_CNT 1
#define DTSLED_NAME "dtsled"
#define LEDON       1
#define LEDOFF      0
struct dtsled_dev{
    dev_t devid;    //设备id
    int major;      //主设备号
    int minor;      //次设备号
    struct cdev cdev; //字符设备结构体
    struct class *class; //类指针
    struct device *device; //设备

    struct device_node *node; 
};

struct dtsled_dev   dtsled; 

//虚拟地址
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;


static void led_switch(u8 sta){
    u32 val = 0;

    if(sta == LEDON){
        val = readl(GPIO1_DR);
        val &= ~(1 << 3);
        writel(val,GPIO1_DR);    //打开灯光
    }
    else{
        val = readl(GPIO1_DR);
        val |= (1 << 3);
        writel(val,GPIO1_DR);    //打开灯光
    }
}

static int dtsled_open(struct inode *inode, struct file *filp){
    filp->private_data = &dtsled ;
    return 0;
}

static int dtsled_release(struct inode *inode, struct file *filp){
    struct dtsled_dev *dev = (struct dtsled_dev *)filp->private_data;
    return 0;
}


static ssize_t dtsled_write (struct file *filp, const char __user *buf, size_t count, loff_t *ops){
    struct dtsled_dev *dev = (struct dtsled_dev *)filp->private_data;
    int ret = 0;
    unsigned char data[1];
    ret = copy_from_user(data,buf,count);
    if(ret < 0){
        return -EFAULT;
    }
    led_switch(data[0]);
    return 0;
}

//字符设备操作结构体
static const struct file_operations dtsled_fops = {
    .owner = THIS_MODULE, 
    .write = dtsled_write,
    .open = dtsled_open,
    .release = dtsled_release,
};


static int __init dtsled_init(void){
    int ret = 0;
    const char *str = NULL;
    u32 regdata[10],val;
    u8 i = 0;

    //注册字符设备
    //申请设备号
    dtsled.major = 0;
    if(dtsled.major){
        dtsled.devid = MKDEV(dtsled.major, 0);
        ret = register_chrdev_region(dtsled.devid, DTSLED_CNT, DTSLED_NAME);
    } else {
        ret = alloc_chrdev_region(&dtsled.devid,0, DTSLED_CNT ,DTSLED_NAME);
        dtsled.major = MAJOR(dtsled.devid);
        dtsled.minor = MINOR(dtsled.devid);
    }
    if(ret < 0){
        printk("dtsled: failed to register char device\n");
        goto err_register;  
    }

    //添加字符设备
    dtsled.cdev.owner = THIS_MODULE;
    cdev_init(&dtsled.cdev, &dtsled_fops);
    ret = cdev_add(&dtsled.cdev,dtsled.devid,DTSLED_CNT);
    if(ret < 0){
        printk("dtsled: failed to add char device\n");
        goto err_add;
    }

    //自动创建设备节点
    dtsled.class = class_create(THIS_MODULE,DTSLED_NAME);
    if(IS_ERR(dtsled.class)){
        ret = PTR_ERR(dtsled.class);
        goto err_class;
    }

    dtsled.device = device_create(dtsled.class, NULL, dtsled.devid, NULL, DTSLED_NAME);
    if(IS_ERR(dtsled.device)){
        ret = PTR_ERR(dtsled.device);
        goto err_device;
    }

    //获取设备树的信息
    dtsled.node = of_find_node_by_path("/led");
    if(!dtsled.node){
        printk("dtsled: can't find /led node\n");
        goto err_node;
    }

    ret = of_property_read_string(dtsled.node,"status",&str);
    if(ret < 0){
        printk("dtsled: can't get status property\n");
        goto err_rs;
    } else {
        printk("dtsled: status = %s\r\n",str); 
    }
#if 0
    ret = of_property_read_u32_array(dtsled.node,"reg",regdata,10);
    if(ret < 0){
        goto err_readu32array;
    } else {
        printk("dtsled: reg = ");
        for(i = 0; i < ret; i++){
            printk("%#x ", regdata[i]);
        }
        printk("\r\n");

    }

    //初始化
    IMX6U_CCM_CCGR1 = ioremap(regdata[0],regdata[1]);
    SW_MUX_GPIO1_IO03 = ioremap(regdata[2],regdata[3]);
    SW_PAD_GPIO1_IO03 = ioremap(regdata[4],regdata[5]);
    GPIO1_DR = ioremap(regdata[6],regdata[7]);
    GPIO1_GDIR = ioremap(regdata[8],regdata[9]);
#endif


    IMX6U_CCM_CCGR1 = of_iomap(dtsled.node,0);
    SW_MUX_GPIO1_IO03 = of_iomap(dtsled.node,1);
    SW_PAD_GPIO1_IO03 = of_iomap(dtsled.node,2);
    GPIO1_DR = of_iomap(dtsled.node,3);
    GPIO1_GDIR = of_iomap(dtsled.node,4);

    //初始化 初始化寄存器
    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26);
    val |= 3 << 26;
    writel(val,IMX6U_CCM_CCGR1);    //设置时钟

    writel(0x5,SW_MUX_GPIO1_IO03);  //设置复用
    writel(0x10B0,SW_PAD_GPIO1_IO03);  //设置电气属性

    val = readl(GPIO1_GDIR);
    val |= 1 << 3;
    writel(val,GPIO1_GDIR);    //设置输出

    val = readl(GPIO1_DR);
    val &= ~(1 << 3);
    writel(val,GPIO1_DR);    //打开灯光

    return 0;


err_readu32array:
err_rs:
err_node:
    device_destroy(dtsled.class, dtsled.devid); 
err_device:
    class_destroy(dtsled.class);
err_class:
    cdev_del(&dtsled.cdev);
err_add:
    unregister_chrdev_region(dtsled.devid, DTSLED_CNT);
err_register:
    return ret;
}
static void __exit dtsled_exit(void){

    unsigned int val = 0;
    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val,GPIO1_DR);    //打开灯光

    iounmap(IMX6U_CCM_CCGR1);
    iounmap(SW_MUX_GPIO1_IO03);
    iounmap(SW_PAD_GPIO1_IO03);
    iounmap(GPIO1_DR);
    iounmap(GPIO1_GDIR);

    device_destroy(dtsled.class, dtsled.devid); 
    class_destroy(dtsled.class);
    cdev_del(&dtsled.cdev);
    unregister_chrdev_region(dtsled.devid, DTSLED_CNT);
}

module_init(dtsled_init);
module_exit(dtsled_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("HPX");  