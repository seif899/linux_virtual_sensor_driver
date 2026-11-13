#ifndef VS_UAPI_H
#define VS_UAPI_H

#include <linux/types.h>   /* __u64, __s32: valid in kernel AND user space */

struct vs_sample {
    __u64 timestamp_ns;
    __s32 temperature_mC;
    __s32 pressure_Pa;
    __s32 accel_mg[3];
    __u32 reserved;         /* explicit padding, always 0 */
};                          /* 8 + 4 + 4 + 12 + 4 = 32 bytes */
#endif /* VS_UAPI_H */