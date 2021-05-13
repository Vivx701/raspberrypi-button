#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include<linux/sysfs.h> 
#include<linux/kobject.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/gpio.h>

#define BUTTON 23
#define DEV_NAME "Push Button"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vivx_Developer <vivx_developer@yahoo.in>");
MODULE_DESCRIPTION("Module for connecting button via GPIO23");
MODULE_VERSION("1.0");



struct device *button_dev;
struct kobject *kobj_ref;
int button_irq;
static unsigned long flags = 0;

ssize_t read_button_values(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int button_value = gpio_get_value(BUTTON);
    return sprintf(buf, "%d", button_value);  
}

struct kobj_attribute button_attr = __ATTR(values, 0444, read_button_values, NULL);

/*
** Module init function
*/
static int __init button_init(void)
{
    button_dev = root_device_register( "button" );
    kobj_ref = &button_dev->kobj;
    
    /*Creating sysfs file */
    if(sysfs_create_file(kobj_ref,  &button_attr.attr)){
        pr_err("Cannot create sysfs file......\n");
        return -1;
    }
    
    if(gpio_request(BUTTON, DEV_NAME) < 0) 
    {
        pr_err("GPIO Button failed.....\n");
        return -1;
    }
    gpio_direction_input(BUTTON);
    return 0;
}



/*
** Module exit function
*/
static void __exit button_exit(void)
{
    gpio_free(BUTTON);
    sysfs_remove_file(kobj_ref,  &button_attr.attr);
    root_device_unregister(button_dev);
}
 
module_init(button_init);
module_exit(button_exit);
 


