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

typedef enum {
    LOG_EMERG,  /* System is unusable */
    LOG_ALERT,  /* Action must be taken immediately */
    LOG_CRIT,   /* Critical conditions */
    LOG_ERR,    /* Error conditions */
    LOG_WARN,   /* Warning conditions */
    LOG_NOTICE, /* Normal but significant condition */
    LOG_INFO,   /* Informational */
    LOG_DEBUG   /* Debug-level messages */
} log_level_t;

/* max length of log messages */
#define LOG_MAX_LEN 256

/* maximum acceptable length of a log filename */
#define LOG_MAX_FILENAME 255

/**
 * Initializes the logging module.
 *
 * Messages whose level is less-than-or-equal-to the provided level
 * will be output. All others will be squelched.
 *
 * filename is a pathname (see open(2)), which will be created if it
 * does not exist and opened in append-only mode.
 *
 * If filename is NULL or longer than LOG_MAX_FILENAME, messages will
 * be written to stderr.
 *
 * Returns true if the logging module initialized successfully, or
 * false if an error occurred. In case of an error, a log message will
 * be output to stderr.
 */
bool log_init(log_level_t level, char *filename);

/**
 * Returns true of the logging module is currently configured to emit
 * messages at the provided level, false otherwise.
 */
bool log_loggable(log_level_t level);

/**
 * Output a message via the logging module with a newline appended.
 *
 * Module users should typically use one of the helper macros, rather
 * than calling this function directly.
 */
void log_write(const char *file, int line, const char *msg, ...)
    __attribute__((nonnull, format(printf, 3, 4)));

/**
 * Output a message to stderr with a newline appended.
 */
void log_stderr(const char *msg, ...)
    __attribute__((nonnull, format(printf, 1, 2)));

/**
 * Output a message to stdout with a newline appended.
 */
void log_stdout(const char *msg, ...)
    __attribute__((nonnull, format(printf, 1, 2)));

/**
 * Deinitializes the logging module, releasing any resources allocated
 * during log_init().
 */
void log_deinit(void);

#define log_emerg(...)                                  \
    do {                                                \
        if (log_loggable(LOG_EMERG)) {                  \
            log_write(__FILE__, __LINE__, __VA_ARGS__); \
        }                                               \
    } while (0)

#define log_alert(...)                                  \
    do {                                                \
        if (log_loggable(LOG_ALERT)) {                  \
            log_write(__FILE__, __LINE__, __VA_ARGS__); \
        }                                               \
    } while (0)

#define log_crit(...)                                   \
    do {                                                \
        if (log_loggable(LOG_CRIT)) {                   \
            log_write(__FILE__, __LINE__, __VA_ARGS__); \
        }                                               \
    } while (0)

#define log_error(...)                                  \
    do {                                                \
        if (log_loggable(LOG_ERR)) {                    \
            log_write(__FILE__, __LINE__, __VA_ARGS__); \
        }                                               \
    } while (0)

#define log_warn(...)                                   \
    do {                                                \
        if (log_loggable(LOG_WARN)) {                   \
            log_write(__FILE__, __LINE__, __VA_ARGS__); \
        }                                               \
    } while (0)

#define log_notice(...)                                 \
    do {                                                \
        if (log_loggable(LOG_NOTICE)) {                 \
            log_write(__FILE__, __LINE__, __VA_ARGS__); \
        }                                               \
    } while (0)

#define log_info(...)                                   \
    do {                                                \
        if (log_loggable(LOG_INFO)) {                   \
            log_write(__FILE__, __LINE__, __VA_ARGS__); \
        }                                               \
    } while (0)

#ifdef ENABLE_DEBUG

#    define log_debug(_level, ...)                          \
        do {                                                \
            if (log_loggable(_level)) {                     \
                log_write(__FILE__, __LINE__, __VA_ARGS__); \
            }                                               \
        } while (0)

#else /* !ENABLE_DEBUG */

#    define log_debug(_level, ...)

#endif /* ENABLE_DEBUG */

END_DECLS
