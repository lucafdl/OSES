
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

// #include <time.h>  serve per l uart

#include <linux/device.h>
#include <asm/uaccess.h>

static dev_t mycdd_dev;

struct cdev mycdd_cdev;

struct class *myclass = NULL;

static char buffer[64];
int i;


ssize_t mycdd_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_INFO "[mycdd] read (count=%d, offset=%d)\n", (int)count, (int)*f_pos );

    return count;
}

ssize_t mycdd_close(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[mycdd] close file i=%d \n", i);
     i=0;

    return 0;
}

static int mycdd_open(struct inode *inode,struct file *file)
{
 printk(KERN_INFO "[mycdd] open file i=%d \n", i);
   i=0;
   return 0;
}

struct file_operations mycdd_fops = {
    .owner = THIS_MODULE,
    .read = mycdd_read,
    .open = mycdd_open,
    .release = mycdd_close,
};

static int __init mycdd_module_init(void)
{
    printk(KERN_INFO "Loading mycdd_module\n");

    alloc_chrdev_region(&mycdd_dev, 0, 1, "mycdd_dev");
    printk(KERN_INFO "%s\n", format_dev_t(buffer, mycdd_dev));

    myclass = class_create(THIS_MODULE, "mycdd_sys");
    device_create(myclass, NULL, mycdd_dev, NULL, "mycdd_dev");

    cdev_init(&mycdd_cdev, &mycdd_fops);
    mycdd_cdev.owner = THIS_MODULE;
    cdev_add(&mycdd_cdev, mycdd_dev, 1);

    return 0;
}

static void __exit mycdd_module_cleanup(void)
{
    printk(KERN_INFO "Cleaning-up mycdd_dev.\n");

    device_destroy(myclass, mycdd_dev );
    cdev_del(&mycdd_cdev);
    class_destroy(myclass);
    unregister_chrdev_region(mycdd_dev, 1);
}

module_init(mycdd_module_init);
module_exit(mycdd_module_cleanup);

MODULE_AUTHOR("Luca Fodale");
MODULE_LICENSE("GPL");
