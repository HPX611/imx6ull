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

	// backlight {
	// 	compatible = "pwm-backlight";
	// 	pwms = <&pwm1 0 5000000>;
	// 	brightness-levels = <0 4 8 16 32 64 128 255>;
	// 	default-brightness-level = <6>;
	// 	status = "okay";
	// };

static int __init dtsof_init(void){
    int ret = 0;
    struct device_node *bl_nd = NULL;
    struct property *comppro = NULL;
    const char *str;
    u32 def_value = 0;
    u32 elems = 0;
    u32 *brival = NULL;
    u8 i = 0;

    //先找到节点
    bl_nd = of_find_node_by_path("/backlight");
    if(bl_nd == NULL){
        ret = -EINVAL;
        goto fail_findnd;
    }

    //获取属性
    comppro = of_find_property(bl_nd,"compatible",NULL);
    if(comppro == NULL){
        ret = -EINVAL;
        goto fail_findpro;
    }
    else{
        printk("compatible = %s\r\n\r\n",(char*)comppro->value);
    }

    ret = of_property_read_string(bl_nd,"status",&str);
    if(ret){
        goto fail_rs;
    }
    else{
        printk("status = %s\r\n\r\n",str);
    }

    ret = of_property_read_u32(bl_nd,"default-brightness-level",&def_value);
    if(ret){
        goto fail_ri;
    }
    else{
        printk("default-brightness-level = %d\r\n\r\n",def_value);
    }


    elems = of_property_count_elems_of_size(bl_nd,"brightness-levels", sizeof(u32));
    if(elems < 0){
        goto fail_readele;
    }
    else{
        printk("brightness-levels elems size = %d\r\n\r\n",elems);
    }
    
    brival = kmalloc(elems*sizeof(u32),GFP_KERNEL);
    if(!brival){
        printk("kmalloc fail\r\n");
        ret = -EINVAL;
        goto fail_mem;
    }


    ret = of_property_read_u32_array(bl_nd,"brightness-levels",brival, elems);
    if(ret < 0){
        ret = -EINVAL;
        goto fail_readarr;
    }
    else{
        for(i = 0;i < elems;i++)
        printk("brightness-levels[%d]= %d\r\n",i,*(brival + i));
    }
    kfree(brival);
    return 0;

fail_readarr:
    kfree(brival);
fail_mem:
fail_readele:
fail_ri: 
fail_rs: 
fail_findpro:
fail_findnd:
    return ret;
}
static void __exit dtsof_exit(void){

}


module_init(dtsof_init);
module_exit(dtsof_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hhh");