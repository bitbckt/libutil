#pragma once

#include <portable/macros.h>
#include <portable/system.h>

BEGIN_DECLS

int _scnprintf(char *buf, size_t size, const char *fmt, ...);
int _vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

#define scnprintf(_s, _n, ...)                          \
    _scnprintf((char *)(_s), (size_t)(_n), __VA_ARGS__)

#define vscnprintf(_s, _n, _f, _a)                      \
    _vscnprintf((char *)(_s), (size_t)(_n), _f, _a)

END_DECLS
