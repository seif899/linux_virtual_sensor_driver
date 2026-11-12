#ifndef VS_CHRDEV_H
#define VS_CHRDEV_H

#include "vs_device.h"

int  vs_chrdev_register(struct vs_device *vs);
void vs_chrdev_unregister(struct vs_device *vs);

#endif