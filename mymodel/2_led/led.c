#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define LED_MAJOR 200
#define LED_NAME  "hhh"

#define LEDON       1
#define LEDOFF      0


//物理地址
#define CCM_CCGR1_BASE              (0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE      (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE      (0X020E02F4)
#define GPIO1_DR_BASE               (0X0209C000)
#define GPIO1_GDIR_BASE             (0X0209C004)

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


static int led_open(struct inode *inode, struct file *filp){
    return 0;
}

static int led_release(struct inode *inode, struct file *filp){
    return 0;
}


static ssize_t led_write (struct file *filp, const char __user *buf, size_t count, loff_t *ops){
    int ret = 0;
    unsigned char data[1];
    ret = copy_from_user(data,buf,count);
    if(ret < 0){
        printk("kernel write fail!\r\n");
        return -EFAULT;
    }
    led_switch(data[0]);
    return 0;
}
static const struct file_operations led_fops = 
{
    .owner = THIS_MODULE,
    .write = led_write,
    .open  = led_open,
    .release = led_release,
};


static int __init led_init(void){
    int ret = 0;
    unsigned int val = 0;
    //初始化 地址映射
    IMX6U_CCM_CCGR1 = ioremap(CCM_CCGR1_BASE,4);
    SW_MUX_GPIO1_IO03 = ioremap(SW_MUX_GPIO1_IO03_BASE,4);
    SW_PAD_GPIO1_IO03 = ioremap(SW_PAD_GPIO1_IO03_BASE,4);
    GPIO1_DR = ioremap(GPIO1_DR_BASE,4);
    GPIO1_GDIR = ioremap(GPIO1_GDIR_BASE,4);

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

    //注册
    ret = register_chrdev(LED_MAJOR,LED_NAME,&led_fops);
    if(ret < 0){
        printk("register chardev failed\r\n");
        return -EIO;
    }

    printk("led init!\r\n");
    return 0;
}
static void __exit led_exit(void){
    unsigned int val = 0;
    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val,GPIO1_DR);    //打开灯光

    iounmap(IMX6U_CCM_CCGR1);
    iounmap(SW_MUX_GPIO1_IO03);
    iounmap(SW_PAD_GPIO1_IO03);
    iounmap(GPIO1_DR);
    iounmap(GPIO1_GDIR);

    unregister_chrdev(LED_MAJOR,LED_NAME);
    printk("led exit!\r\n");
}


module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hhh");
