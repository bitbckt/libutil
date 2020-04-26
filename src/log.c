#include <errno.h>
#include <portable/system.h>

#include "log.h"
#include "str.h"
#include "xwrite.h"

/* number of errors during logging */
static uint32_t log_nerror = 0;

void
_log_stderr(const char *fmt, ...)
{
    size_t len, size;
    int errno_save;
    char buf[4 * LOG_MAX_LEN];
    va_list args;
    ssize_t n;

    errno_save = errno;
    len = 0;                /* length of output buffer */
    size = 4 * LOG_MAX_LEN; /* size of output buffer */

    va_start(args, fmt);
    len += vscnprintf(buf, size, fmt, args);
    va_end(args);

    buf[len++] = '\n';

    n = xwrite(STDERR_FILENO, buf, len);
    if (n < 0) {
        log_nerror += 1;
    }

    errno = errno_save;
}
