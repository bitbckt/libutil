#include <errno.h>
#include <portable/system.h>

#include "xwrite.h"

ssize_t
xwrite(int fd, const void *buffer, size_t size)
{
    size_t total;
    ssize_t status;
    unsigned int count = 0;

    if (size == 0) {
        return 0;
    }

    /* Abort the write if we try ten times with no forward progress. */
    for (total = 0; total < size; total += status) {
        if (++count > 10) {
            break;
        }

        status = write(fd, (const char *) buffer + total, size - total);

        if (status > 0) {
            count = 0;
        }

        if (status < 0) {
            if (errno != EINTR) {
                break;
            }
            status = 0;
        }
    }

    if (total < size) {
        return -1;
    } else {
        return (ssize_t)total;
    }
}
