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

#include <util/buffer.h>

#include "assert.h"
#include "util-private.h"
#include "xmalloc.h"

#ifdef ASSERTS
#    define DBUF_MAGIC 0xdeadbeef
#endif

/**
 * Double buffer mechanics:
 *
 * dbuf points to the header, so that buffer overflow can be easily
 * detected by asserting on magic bytes during dbuf_get()/dbuf_put().
 *
 * dbuf->read and dbuf->write always point to the first byte of either
 * buffer a or b.
 *
 * dbuf->pos starts at dbuf->read and increments for each byte read by
 * dbuf_get() until it reaches the last readable byte at dbuf->limit -
 * 1.
 *
 * dbuf->last starts at dbuf->write and increments for each byte
 * written by dbuf_put() until it reaches either buffer b (if
 * dbuf->write == buffer a) or dbuf (if dbuf->write == buffer b).
 *
 * When dbuf->pos == dbuf->limit, no readable bytes exist in the read
 * buffer. If any unread bytes exists in the write buffer, the two
 * buffers are swapped.
 *
 * +-------------------------------------------+
 * |        dbuf data        |   dbuf header   |
 * +-------------------------|  (struct dbuf)  |
 * |  buffer a  |  buffer b  |                 |
 * +-------------------------------------------+
 *                           ^
 *                           \
 *                            dbuf
 *
 * +-------------+         +--------------+
 * | read buffer |         | write buffer |
 * +-------------+         +--------------+
 * ^   ^       ^           ^      ^
 * |   \       |           |      \
 * \   dbuf->pos           \       dbuf->last
 *  dbuf->read \            dbuf->write
 *              dbuf->limit
 */
struct dbuf {
#ifdef ASSERTS
    uint32_t magic; /* dbuf magic (const) */
#endif
    uint8_t *pos;   /* read marker */
    uint8_t *limit; /* read limit */
    uint8_t *last;  /* write marker */
    uint8_t *read;  /* start of read buffer */
    uint8_t *write; /* start of write buffer */
};

UTIL_EXPORT struct dbuf *
dbuf_init(size_t size)
{
    struct dbuf *dbuf;
    uint8_t *    buf;

    buf = xzalloc(sizeof(struct dbuf) + size * 2);
    if (buf == NULL) {
        return NULL;
    }

    dbuf = (struct dbuf *)(buf + size * 2);

#ifdef ASSERTS
    dbuf->magic = DBUF_MAGIC;
#endif

    dbuf->read = buf;
    dbuf->pos = buf;
    dbuf->limit = buf;

    dbuf->write = buf + size;
    dbuf->last = dbuf->write;

    return dbuf;
}

UTIL_EXPORT bool
dbuf_put(struct dbuf *dbuf, uint8_t byte)
{
    ASSERT(dbuf->magic == DBUF_MAGIC);

    /* out of writable capacity, wait for reader */
    if (dbuf->last == dbuf->read || dbuf->last == (uint8_t *)dbuf) {
        return false;
    }

    *dbuf->last = byte;
    ++dbuf->last;

    return true;
}

static void
_dbuf_swap(struct dbuf *dbuf)
{
    uint8_t *tmp = dbuf->read;

    dbuf->read = dbuf->write;
    dbuf->pos = dbuf->read;
    dbuf->limit = dbuf->last;

    dbuf->write = tmp;
    dbuf->last = dbuf->write;
}

UTIL_EXPORT bool
dbuf_get(struct dbuf *dbuf, uint8_t *byte)
{
    ASSERT(dbuf->magic == DBUF_MAGIC);

    if (dbuf->limit > dbuf->pos ||  /* unread bytes in the read buffer */
        dbuf->last > dbuf->write) { /* unread bytes in the write buffer */

        /* empty/exhausted read buffer */
        if (dbuf->limit == dbuf->pos) {
            _dbuf_swap(dbuf);
        }

        *byte = *dbuf->pos;
        ++dbuf->pos;

        return true;
    }

    /*
     * nothing has been written to either buffer since the last read.
     */
    return false;
}

UTIL_EXPORT void
dbuf_deinit(struct dbuf *dbuf)
{
    if (dbuf->read < dbuf->write) {
        xfree(dbuf->read);
    } else {
        xfree(dbuf->write);
    }
}
