#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/init.h>

#include "vs_device.h"
#include "vs_chrdev.h"
#include "vs_sampler.h"

static struct vs_device vs_dev;

static int __init vs_init(void)
{
    int ret;

    ret = vs_chrdev_register(&vs_dev);
    if (ret)
        return ret;

    ret = vs_sampler_start(&vs_dev, VS_DEFAULT_RATE_HZ);
    if (ret)
        goto err_chrdev;

    return 0;

err_chrdev:
    vs_chrdev_unregister(&vs_dev);
    return ret;
}

static void __exit vs_exit(void)
{
    vs_sampler_stop(&vs_dev);          /* reverse order of init */
    vs_chrdev_unregister(&vs_dev);
    pr_info("unloaded\n");
}

module_init(vs_init);
module_exit(vs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seif");
MODULE_DESCRIPTION("Virtual sensor character driver");