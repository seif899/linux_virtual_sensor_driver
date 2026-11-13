#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "vs_uapi.h"

int main(void)
{
    int fd = open("/dev/virtual_sensor", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct vs_sample s;
    for (;;) {
        ssize_t n = read(fd, &s, sizeof(s));
        if (n == (ssize_t)sizeof(s)) {
            printf("t=%llu ns  temp=%d.%03d C  press=%d Pa  accel=(%d,%d,%d) mg\n",
                   (unsigned long long)s.timestamp_ns,
                   s.temperature_mC / 1000, abs(s.temperature_mC % 1000),
                   s.pressure_Pa, s.accel_mg[0], s.accel_mg[1], s.accel_mg[2]);
        } else if (n < 0 && errno == EAGAIN) {
            usleep(100 * 1000);       /* no data yet: busy-poll (ugly, will fix) */
        } else {
            perror("read");
            break;
        }
    }
    close(fd);
    return 0;
}