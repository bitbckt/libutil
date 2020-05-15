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

/* maximum acceptable length of a log filename */
#define LOG_MAX_FILENAME 255

/**
 * Initializes the logging module.
 *
 * Messages whose level is greater than the provided level will be
 * output. All others will be squelched.
 *
 * filename is a pathname (see open(2)), which will be created if it
 * does not exist and opened in append-only mode.
 *
 * If filename is NULL or longer than LOG_MAX_FILENAME, messages will
 * be written to stderr.
 *
 * Returns 0 if the logging module initialized successfully, or -1 if
 * an error occurred. In case of an error, a log message will be
 * output to stderr.
 */
int log_init(int level, char *filename);

/**
 * Returns true of the logging module is currently configured to emit
 * messages at the provided level, false otherwise.
 */
bool log_loggable(int level);

/**
 * Deinitializes the logging module, releasing any resources allocated
 * during log_init().
 */
void log_deinit(void);

void _log(const char *file, int line, const char *fmt, ...)
    __attribute__((nonnull, format(printf, 3, 4)));
void _log_stderr(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#ifdef ENABLE_DEBUG

#    define log_debug(_level, ...)                     \
        do {                                           \
            if (log_loggable(_level)) {                \
                _log(__FILE__, __LINE__, __VA_ARGS__); \
            }                                          \
        } while (0)

#else /* !ENABLE_DEBUG */

#    define log_debug(_level, ...)

#endif /* ENABLE_DEBUG */

#define log_error(...)                             \
    do {                                           \
        if (log_loggable(LOG_ERR)) {               \
            _log(__FILE__, __LINE__, __VA_ARGS__); \
        }                                          \
    } while (0)

#define log_stderr(...)           \
    do {                          \
        _log_stderr(__VA_ARGS__); \
    } while (0)

END_DECLS
