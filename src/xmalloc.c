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

#include <portable/system.h>
#include <util/log.h>

#include "xmalloc.h"

#include "assert.h"
#include "util-private.h"

void *
_xcalloc(size_t nmemb, size_t size, const char *name, int line)
{
    return _xzalloc(nmemb * size, name, line);
}

void *
_xmalloc(size_t size, const char *name, int line)
{
    void *p;

    ASSERT(size != 0);

    p = malloc(size);
    if (p == NULL) {
        log_error("malloc(%zu) failed @ %s:%d", size, name, line);
    } else {
        log_debug(LOG_DEBUG, "malloc(%zu) at %p @ %s:%d", size, p, name, line);
    }

    return p;
}

void *
_xrealloc(void *ptr, size_t size, const char *name, int line)
{
    void *p;

    ASSERT(size != 0);

    p = realloc(ptr, size);
    if (p == NULL) {
        log_error("realloc(%zu) failed @ %s:%d", size, name, line);
    } else {
        log_debug(LOG_DEBUG, "realloc(%zu) at %p @ %s:%d", size, p, name,
                  line);
    }

    return p;
}

void *
_xzalloc(size_t size, const char *name, int line)
{
    void *p;

    p = _xmalloc(size, name, line);
    if (p != NULL) {
        /*
         * Call to memset is insecure as it does not provide security
         * checks introduced in the C11 standard
         */
        memset(p, 0, size); /* NOLINT */
    }

    return p;
}

void
_xfree(void *ptr, const char *name, int line)
{
#if !defined(ENABLE_DEBUG)
    UNUSED(name);
    UNUSED(line);
#endif

    ASSERT(ptr != NULL);
    log_debug(LOG_DEBUG, "free(%p) @ %s:%d", ptr, name, line);
    free(ptr);
}
