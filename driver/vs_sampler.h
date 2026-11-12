#ifndef VS_SAMPLER_H
#define VS_SAMPLER_H

#include "vs_device.h"

#define VS_DEFAULT_RATE_HZ 2
#define VS_MIN_RATE_HZ     1
#define VS_MAX_RATE_HZ     100

int  vs_sampler_start(struct vs_device *vs, u32 rate_hz);
void vs_sampler_stop(struct vs_device *vs);

#endif