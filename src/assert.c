#include <portable/system.h>

#include "assert.h"
#include "log.h"
#include "util-private.h"
#include "xmalloc.h"

void
_assert(const char *cond, const char *file, int line, int panic)
{
    /* TODO: debug log */
    UNUSED(cond);
    UNUSED(file);
    UNUSED(line);
    
    if (panic) {
        stacktrace(1);
        abort();
    }
}

void
stacktrace(int skip)
{
#ifdef HAVE_BACKTRACE
    void *stack[64];
    char **symbols;
    int size, i, j;

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
