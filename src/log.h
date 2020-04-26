#pragma once

#include <portable/macros.h>

BEGIN_DECLS

#define LOG_MAX_LEN 256 /* max length of log messages */

void _log_stderr(const char *fmt, ...);

#define log_stderr(...) do {                                            \
        _log_stderr(__VA_ARGS__);                                       \
} while (0)

END_DECLS
