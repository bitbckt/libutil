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
#include <portable/system.h>

#include "xwrite.h"

/* maximum number of write(2) attempts before giving up */
#define MAX_ATTEMPTS 10

ssize_t
xwrite(int fd, const void *buffer, size_t size)
{
    size_t       total;
    ssize_t      status;
    unsigned int count = 0;

    if (size == 0) {
        return 0;
    }

    /*
     * Abort the write if we try MAX_ATTEMPTS times with no forward progress.
     */
    for (total = 0; total < size; total += status) {
        if (++count > MAX_ATTEMPTS) {
            break;
        }

        status = write(fd, (const char *)buffer + total, size - total);

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
    }

    return (ssize_t)total;
}
