#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>


#define CHRDEVBASE_MAJOR    200     //主设备号
#define CHRDEVBASE_NAME     "chrdevbase"    //设备名字

static char readbuf[100];   //读缓冲
static char writebuf[100];   //写缓冲
static char data[] = {"kernel data!"};  

static int chrdevbase_open(struct inode *inode,struct file *filp){
    printk("chrdevbase_open\r\n");
    return 0;
}
static int chrdevbase_release(struct inode *inode,struct file *filp){
    printk("chrdevbase_release\r\n");
    return 0;
}

static ssize_t chrdevbase_read(struct file *filp,__user char *buf,size_t count,loff_t *ppos){
    // printk("chrdevbase_read\r\n");
    memcpy(readbuf,data,sizeof(data));

    //第一个都是目的的数据，第二个才是数据源，第三个是长度
    copy_to_user(buf,readbuf,count);
    return 0;
}
static ssize_t chrdevbase_write(struct file *filp,const char __user *buf,size_t count,loff_t *ppos){
    // printk("chrdevbase_write\r\n");
    int ret = 0;
    //第一个都是目的的数据，第二个才是数据源，第三个是长度
    ret = copy_from_user(writebuf,buf,count);
    if(ret == 0){
        printk("kernel recevdata:%s\r\n" ,writebuf);
    }
    return 0;
}

static struct file_operations chrdevbase_fops={
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .release = chrdevbase_release,
    .read = chrdevbase_read,
    .write = chrdevbase_write,
};


static int __init chrdevbase_init(void){

    int ret;
    printk("chrdevbase_init\r\n");

    //注册字符设备
    ret = register_chrdev(CHRDEVBASE_MAJOR,CHRDEVBASE_NAME,&chrdevbase_fops);
    if(ret < 0){
        printk("register_chrdev error!\r\n");
    }
    
    return 0;
}

static void __exit chrdevbase_exit(void){
    printk("chrdevbase_exit\r\n");

    //注销字符设备
    unregister_chrdev(CHRDEVBASE_MAJOR,CHRDEVBASE_NAME);

}



// 模块的注册和退出
module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDH"); 
