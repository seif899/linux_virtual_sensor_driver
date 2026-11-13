#ifndef VS_RINGBUF_H
#define VS_RINGBUF_H

#include <linux/types.h>
#include <linux/spinlock.h>
#include "vs_uapi.h"
#include "vs_ringbuf.h"

struct vs_ringbuf {
    struct vs_sample *buf;
    u32               size;     /* capacity; must be a power of two */
    u32               head;     /* free-running write counter */
    u32               tail;     /* free-running read counter  */
    u64               dropped;  /* samples overwritten because buffer was full */
    spinlock_t        lock;     /* protects everything above */
};

int  vs_ringbuf_init(struct vs_ringbuf *rb, u32 size);
void vs_ringbuf_destroy(struct vs_ringbuf *rb);
bool vs_ringbuf_push(struct vs_ringbuf *rb, const struct vs_sample *s); /* true if it dropped one */
bool vs_ringbuf_pop(struct vs_ringbuf *rb, struct vs_sample *s);        /* false if empty */

#endif