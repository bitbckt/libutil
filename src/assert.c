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

#include "assert.h"

#include "log.h"
#include "util-private.h"
#include "xmalloc.h"

void
_assert(const char *cond, const char *file, int line, bool panic)
{
    log_error("assert '%s' failed @ (%s, %d)", cond, file, line);
    if (panic) {
        stacktrace(1);
        abort();
    }
}

void
stacktrace(int skip)
{
#ifdef HAVE_BACKTRACE
    void * stack[64];
    char **symbols;
    int    size, i, j;

    size = backtrace(stack, 64);
    symbols = backtrace_symbols(stack, size);
    if (symbols == NULL) {
        return;
    }

    skip++; /* skip this frame */

    for (i = skip, j = 0; i < size; i++, j++) {
        log_stderr("[%d] %s", j, symbols[i]);
    }

    xfree(symbols);
#else
    UNUSED(skip);
#endif /* !HAVE_BACKTRACE */
}
