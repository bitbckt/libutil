#pragma once

#include <portable/macros.h>

BEGIN_DECLS

void *_xcalloc(size_t nmemb, size_t size, const char *name, int line);
void *_xmalloc(size_t size, const char *name, int line);
void *_xrealloc(void *ptr, size_t size, const char *name, int line);
void *_xzalloc(size_t size, const char *name, int line);
void _xfree(void *ptr, const char *name, int line);

#define xcalloc(_n, _s)                         \
    _xcalloc((size_t)(_n), (size_t)(_s), __FILE, __LINE__)

#define xmalloc(_s)                             \
    _xmalloc((size_t)(_s), __FILE__, __LINE__)

#define xrealloc(_p, _s)                        \
    _xrealloc(_p, (size_t)(_s), __FILE__, __LINE__)

#define xzalloc(_s)                             \
    _xzalloc((size_t)(_s), __FILE__, __LINE__)

#define xfree(_p)                               \
    do {                                        \
        _xfree(_p, __FILE__, __LINE__);         \
        (_p) = NULL;                            \
    } while(0)

END_DECLS
