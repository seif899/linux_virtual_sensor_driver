#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/log2.h>
#include <linux/errno.h>

#include "vs_ringbuf.h"

int vs_ringbuf_init(struct vs_ringbuf *rb, u32 size)
{
    if (!is_power_of_2(size))
        return -EINVAL;

    rb->buf = kcalloc(size, sizeof(*rb->buf), GFP_KERNEL);
    if (!rb->buf)
        return -ENOMEM;

    rb->size    = size;
    rb->head    = 0;
    rb->tail    = 0;
    rb->dropped = 0;
    spin_lock_init(&rb->lock);
    return 0;
}

void vs_ringbuf_destroy(struct vs_ringbuf *rb)
{
    pr_info("ring buffer freed (dropped %llu samples in total)\n", rb->dropped);
    kfree(rb->buf);
    rb->buf = NULL;
}

bool vs_ringbuf_push(struct vs_ringbuf *rb, const struct vs_sample *s)
{
    bool dropped = false;

    spin_lock_bh(&rb->lock);

    if (rb->head - rb->tail == rb->size) {   /* full: overwrite the OLDEST */
        rb->tail++;
        rb->dropped++;
        dropped = true;
    }
    rb->buf[rb->head & (rb->size - 1)] = *s;
    rb->head++;

    spin_unlock_bh(&rb->lock);
    return dropped;
}

bool vs_ringbuf_pop(struct vs_ringbuf *rb, struct vs_sample *s)
{
    bool ok = false;

    spin_lock_bh(&rb->lock);

    if (rb->head != rb->tail) {
        *s = rb->buf[rb->tail & (rb->size - 1)];
        rb->tail++;
        ok = true;
    }

    spin_unlock_bh(&rb->lock);
    return ok;
}