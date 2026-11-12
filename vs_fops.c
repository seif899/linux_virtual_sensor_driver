#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#include "vs_device.h"
#include "vs_fops.h"

static int vs_open(struct inode *inode, struct file *filp)
{
    /* Recover our vs_device from the cdev embedded inside it, and stash it
       in the file so every later call (read, ioctl...) can reach it. */
    struct vs_device *vs = container_of(inode->i_cdev, struct vs_device, cdev);

    filp->private_data = vs;
    pr_info("open (pid %d)\n", current->pid);
    return 0;
}

static int vs_release(struct inode *inode, struct file *filp)
{
    pr_info("release (pid %d)\n", current->pid);
    return 0;
}

static ssize_t vs_read(struct file *filp, char __user *buf,
                       size_t count, loff_t *ppos)
{
    static const char msg[] = "hello from virtual_sensor\n";

    return simple_read_from_buffer(buf, count, ppos, msg, sizeof(msg) - 1);
}

const struct file_operations vs_fops = {
    .owner   = THIS_MODULE,
    .open    = vs_open,
    .release = vs_release,
    .read    = vs_read,
};