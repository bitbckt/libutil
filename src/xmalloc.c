#include <portable/system.h>

#include "assert.h"
#include "log.h"
#include "util-private.h"
#include "xmalloc.h"

void *
_xcalloc(size_t nmemb, size_t size, const char *name, int line)
{
    return _xzalloc(nmemb * size, name, line);
}

void *
_xmalloc(size_t size, const char *file, int line)
{
    void *p;

    UNUSED(file);
    UNUSED(line);

    ASSERT(size != 0);

    /* TODO: log if p == NULL */
    p = malloc(size);
    return p;
}

void *
_xrealloc(void *ptr, size_t size, const char *name, int line)
{
    void *p;

    UNUSED(name);
    UNUSED(line);

    ASSERT(size != 0);

    /* TODO: log if p == NULL */
    p = realloc(ptr, size);
    return p;
}

void *
_xzalloc(size_t size, const char *name, int line)
{
    void *p;

    p = _xmalloc(size, name, line);
    if (p != NULL) {
        memset(p, 0, size);
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
