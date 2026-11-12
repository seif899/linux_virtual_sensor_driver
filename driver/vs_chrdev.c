#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/err.h>

#include "vs_chrdev.h"
#include "vs_fops.h"

int vs_chrdev_register(struct vs_device *vs){
    int ret;

    ret = alloc_chrdev_region(&vs->devt, 0, 1, VS_DEVICE_NAME);
    if (ret)
        return ret;

    dev_init(&vs->cdev, &vs_fops);
    vs->cdev.owner = THIS_MODULE;
    ret = cdev_add(&vs->cdev, vs->devt, 1);
    if (ret)
        goto err_region;

    vs->class = class_create(VS_CLASS_NAME);       /* 6.4+ signature */
    if (IS_ERR(vs->class)) {
        ret = PTR_ERR(vs->class);
        goto err_cdev;
    }

    vs->dev = device_create(vs->class, NULL, vs->devt, NULL, VS_DEVICE_NAME);
    if (IS_ERR(vs->dev)) {
        ret = PTR_ERR(vs->dev);
        goto err_class;
    }

    pr_info("registered, major=%d minor=%d\n",
            MAJOR(vs->devt), MINOR(vs->devt));
    return 0;

err_class:
    class_destroy(vs->class);
err_cdev:
    cdev_del(&vs->cdev);
err_region:
    unregister_chrdev_region(vs->devt, 1);
    return ret;
}

void vs_chrdev_unregister(struct vs_device *vs)
{
    device_destroy(vs->class, vs->devt);    /* reverse order of register */
    class_destroy(vs->class);
    cdev_del(&vs->cdev);
    unregister_chrdev_region(vs->devt, 1);
}