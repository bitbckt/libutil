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

#pragma once

#include <portable/macros.h>
#include <portable/stdbool.h>

BEGIN_DECLS

#define LOG_EMERG   0 /* System is unusable */
#define LOG_ALERT   1 /* Action must be taken immediately */
#define LOG_CRIT    2 /* Critical conditions */
#define LOG_ERR     3 /* Error conditions */
#define LOG_WARNING 4 /* Warning conditions */
#define LOG_NOTICE  5 /* Normal but significant condition */
#define LOG_INFO    6 /* Informational */
#define LOG_DEBUG   7 /* Debug-level messages */

#define LOG_MAX_LEN 256 /* max length of log messages */

int log_init(int level, char *filename);
bool log_loggable(int level);
void log_deinit(void);
void _log(const char *file, int line, const char *fmt, ...);
void _log_stderr(const char *fmt, ...);

#ifdef ENABLE_DEBUG

#define log_debug(_level, ...) do {                                     \
        if (log_loggable(_level) != 0) {                                \
            _log(__FILE__, __LINE__, __VA_ARGS__);                      \
        }                                                               \
    } while (0)

#else /* !ENABLE_DEBUG */

#define log_debug(_level, ...)

#endif /* ENABLE_DEBUG */

#define log_error(...) do {                                             \
        if (log_loggable(LOG_ERR) != 0) {                               \
            _log(__FILE__, __LINE__, __VA_ARGS__);                      \
        }                                                               \
    } while (0)

#define log_stderr(...) do {                                            \
        _log_stderr(__VA_ARGS__);                                       \
    } while (0)

END_DECLS
