#pragma once

#include <portable/macros.h>

BEGIN_DECLS

/*
 * Like the non-x versions of the same function, but keep writing until either
 * the write is not making progress or there's a real error.  Handle partial
 * writes and EINTR/EAGAIN errors.
 */
ssize_t xwrite(int fd, const void *buffer, size_t size)
    __attribute__((__nonnull__));

END_DECLS
