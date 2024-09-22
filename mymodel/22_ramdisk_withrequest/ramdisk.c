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
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>


#define RAMDISK_SIZE (2*1024*1024)
#define RAMDISK_NAME "ramdisk"
#define RAMDISK_MINOR 3     //有3个分区


struct ramdisk_dev { 
    int major;
    unsigned char *ramdiskbuf;
    spinlock_t lock;
    struct gendisk *gendisk;
    struct request_queue *queue;
};

struct ramdisk_dev ramdisk;

int ramdisk_open(struct block_device *dev,fmode_t mode)
{
    printk("ramdisk open\r\n");
    return 0;
}

void ramdisk_release(struct gendisk *disk,fmode_t mode)
{
    printk("ramdisk release\r\n");
}

int ramdisk_getgeo(struct block_device *dev,struct hd_geometry *geo)
{
    geo->heads = 2;
    geo->cylinders = 32;
    geo->sectors = RAMDISK_SIZE / (2*32*512);
    return 0;
}

static struct block_device_operations ramdisk_fops = {
    .owner = THIS_MODULE,
    .open = ramdisk_open,
    .release = ramdisk_release,
    .getgeo = ramdisk_getgeo,
};

static void ramdisk_transfer(struct request *req)
{
    unsigned long start = blk_rq_pos(req) << 9;
    unsigned long len = blk_rq_cur_bytes(req);

    void *buffer = bio_data(req->bio);

    if(rq_data_dir(req) == READ)
        memcpy(buffer,ramdisk.ramdiskbuf + start,len);
    else if(rq_data_dir(req) == WRTIE)
        memcpy(ramdisk.ramdiskbuf + start,buffer,len);
}

void ramdisk_request_fn(struct request_queue *q)
{
    int err = 0;
    struct request *req;
    
    req = blk_fetch_request(q);
    while(req != NULL)
    {
        ramdisk_transfer(req);
        
    }
}