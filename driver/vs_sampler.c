#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>
#include <linux/random.h>
#include <linux/timer.h>
#include <linux/errno.h>

#include "vs_device.h"
#include "vs_sampler.h"
#include "vs_uapi.h"

/* Fake sensor: values wobble randomly around realistic baselines. */
static void vs_sample_generate(struct vs_sample *s)
{
    s->timestamp_ns    = ktime_get_ns();
    s->temperature_mC  = 25000  + (s32)get_random_u32_below(2001)  - 1000;
    s->pressure_Pa     = 101325 + (s32)get_random_u32_below(201)   - 100;
    s->accel_mg[0]     = (s32)get_random_u32_below(201)  - 100;
    s->accel_mg[1]     = (s32)get_random_u32_below(201)  - 100;
    s->accel_mg[2]     = 1000 + (s32)get_random_u32_below(201) - 100; /* gravity */
    s->reserved = 0;
}

/* Runs in SOFTIRQ (atomic) context: no sleeping, no mutexes, no copy_to_user. */
static void vs_sampler_fn(struct timer_list *t)
{
    struct vs_device *vs = from_timer(vs, t, timer);
    struct vs_sample s;

    vs_sample_generate(&s);
    vs_ringbuf_push(&vs->rb, &s);

    mod_timer(&vs->timer, jiffies + vs->period_jiffies);
}

int vs_sampler_start(struct vs_device *vs, u32 rate_hz)
{
    if (rate_hz < VS_MIN_RATE_HZ || rate_hz > VS_MAX_RATE_HZ)
        return -EINVAL;

    vs->rate_hz = rate_hz;
    vs->period_jiffies = max(1UL, msecs_to_jiffies(1000 / rate_hz));

    timer_setup(&vs->timer, vs_sampler_fn, 0);
    mod_timer(&vs->timer, jiffies + vs->period_jiffies);

    pr_info("sampler started at %u Hz (%lu jiffies)\n",
            vs->rate_hz, vs->period_jiffies);
    return 0;
}

void vs_sampler_stop(struct vs_device *vs)
{
    /* Waits for a running callback to finish AND prevents re-arming. */
    del_timer_sync(&vs->timer);
    pr_info("sampler stopped\n");
}