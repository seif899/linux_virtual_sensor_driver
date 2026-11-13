#ifndef VS_DEVICE_H
#define VS_DEVICE_H

#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include "vs_ringbuf.h"

#define VS_DEVICE_NAME "virtual_sensor"
#define VS_CLASS_NAME  "vsensor"

struct vs_device {
    dev_t devt;
    struct cdev cdev;
    struct class *class;
    struct device *dev;
    // add: ring buffer, locks, wait queue, timer, stats ...
    /* sampler */
    struct timer_list timer;
    unsigned long     period_jiffies;
    u32               rate_hz;

    /* data path */
    struct vs_ringbuf rb;

};

#endif