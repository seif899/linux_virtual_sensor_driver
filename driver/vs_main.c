#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/init.h>

#include "vs_device.h"
#include "vs_chrdev.h"

static struct vs_device vs_dev;     /* the one device instance */

static int __init vs_init(void)
{
    int ret;

    ret = vs_chrdev_register(&vs_dev);
    if (ret)
        return ret;

    /* later steps add: vs_ringbuf_init(), vs_sampler_start(), ... here */
    return 0;
}

static void __exit vs_exit(void)
{
    /* later: stop things in REVERSE order of init */
    vs_chrdev_unregister(&vs_dev);
    pr_info("unloaded\n");
}

module_init(vs_init);
module_exit(vs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seif");
MODULE_DESCRIPTION("Virtual sensor character driver");