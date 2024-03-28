#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // 包含 copy_to_user() 和 copy_from_user() 函数的头文件

#define BUFFER_SIZE 1024

static char kernel_buffer[BUFFER_SIZE];

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t count, loff_t *ppos)
{
    int bytes_read = 0;
    if (*ppos >= BUFFER_SIZE)
        return 0;

    if (*ppos + count > BUFFER_SIZE)
        count = BUFFER_SIZE - *ppos;

    if (copy_to_user(user_buffer, kernel_buffer + *ppos, count) != 0)
        return -EFAULT;

    *ppos += count;
    bytes_read = count;

    printk(KERN_INFO "Read %d bytes from device\n", bytes_read);

    return bytes_read;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
    int bytes_written = 0;
    if (*ppos >= BUFFER_SIZE)
        return -ENOSPC;

    if (*ppos + count > BUFFER_SIZE)
        count = BUFFER_SIZE - *ppos;

    if (copy_from_user(kernel_buffer + *ppos, user_buffer, count) != 0)
        return -EFAULT;

    *ppos += count;
    bytes_written = count;

    printk(KERN_INFO "Written %d bytes to device\n", bytes_written);

    return bytes_written;
}

static struct file_operations my_fops = {
    .open = my_open,
    .read = my_read,
    .write = my_write,
};

static int __init my_init(void)
{
    int ret;

    ret = register_chrdev(0, "my_device", &my_fops);
    if (ret < 0)
    {
        printk(KERN_ALERT "Failed to register device: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "Device registered\n");

    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev(0, "my_device");
    printk(KERN_INFO "Device unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple example Linux module.");
