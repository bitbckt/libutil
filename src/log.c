/*
 * libutil - C utilities
 *
 * Copyright (C) 2020 Brandon Mitchell <brandon@thewholedoubt.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <fcntl.h>
#include <portable/stdbool.h>
#include <portable/system.h>
#include <sys/time.h>
#include <time.h>

#include "log.h"
#include "str.h"
#include "util-private.h"
#include "xwrite.h"

/* maximum acceptable length of a log filename */
#define FILENAME_MAX_LEN 255

/* number of errors during logging */
static uint32_t log_nerror = 0;

static struct logger {
    char *name;  /* log file name */
    int  level;  /* log level */
    int  fd;     /* log file descriptor */
} logger;

int
log_init(int level, char *filename)
{
    struct logger *l = &logger;

    l->level = MAX(LOG_EMERG, MIN(level, LOG_DEBUG));
    l->name = filename;
    if (filename == NULL || !strnlen(filename, FILENAME_MAX_LEN)) {
        l->fd = STDERR_FILENO;
    } else {
        l->fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (l->fd < 0) {
            log_stderr("opening log file '%s' failed: %s", filename,
                       strerror(errno));
            return -1;
        }
    }

    return 0;
}

bool
log_loggable(int level)
{
    struct logger *l = &logger;

    if (level > l->level) {
        return true;
    }

    return false;
}

void
log_deinit(void)
{
    struct logger *l = &logger;

    if (l->fd < 0 || l->fd == STDERR_FILENO) {
        return;
    }

    close(l->fd);
}

void
_log(const char *file, int line, const char *fmt, ...)
{
    struct logger *l = &logger;
    int len, size, errno_save;
    char buf[LOG_MAX_LEN];
    va_list args;
    ssize_t n;
    struct timeval tv;

    if (l->fd < 0) {
        return;
    }

    errno_save = errno;
    len = 0;            /* length of output buffer */
    size = LOG_MAX_LEN; /* size of output buffer */

    gettimeofday(&tv, NULL);
    buf[len++] = '[';
    len += strftime(buf + len, size - len, "%Y-%m-%d %H:%M:%S.", localtime(&tv.tv_sec));
    len += scnprintf(buf + len, size - len, "%03ld", tv.tv_usec/1000);
    len += scnprintf(buf + len, size - len, "] %s:%d ", file, line);

    va_start(args, fmt);
    len += vscnprintf(buf + len, size - len, fmt, args);
    va_end(args);

    buf[len++] = '\n';

    n = xwrite(l->fd, buf, len);
    if (n < 0) {
        log_nerror++;
    }

    errno = errno_save;
}

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
