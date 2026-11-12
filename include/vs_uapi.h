#ifndef VS_UAPI_H
#define VS_UAPI_H

#include <linux/types.h>   /* __u64, __s32: valid in kernel AND user space */

struct vs_sample {
    __u64 timestamp_ns;
    __s32 temperature_mC;   /* milli-degrees Celsius: 25000 = 25.000 C */
    __s32 pressure_Pa;
    __s32 accel_mg[3];      /* x, y, z in milli-g: 1000 = 1 g */
};

#endif /* VS_UAPI_H */