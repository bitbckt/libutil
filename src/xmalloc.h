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

BEGIN_DECLS

void *_xcalloc(size_t nmemb, size_t size, const char *name, int line)
    __attribute__((nonnull, warn_unused_result));
void *_xmalloc(size_t size, const char *name, int line)
    __attribute__((nonnull, malloc, warn_unused_result));
void *_xrealloc(void *ptr, size_t size, const char *name, int line)
    __attribute__((nonnull, warn_unused_result));
void *_xzalloc(size_t size, const char *name, int line)
    __attribute__((nonnull, warn_unused_result));
void _xfree(void *ptr, const char *name, int line) __attribute__((nonnull));

#define xcalloc(_n, _s) _xcalloc((size_t)(_n), (size_t)(_s), __FILE, __LINE__)

#define xmalloc(_s) _xmalloc((size_t)(_s), __FILE__, __LINE__)

#define xrealloc(_p, _s) _xrealloc(_p, (size_t)(_s), __FILE__, __LINE__)

#define xzalloc(_s) _xzalloc((size_t)(_s), __FILE__, __LINE__)

#define xfree(_p)                       \
    do {                                \
        _xfree(_p, __FILE__, __LINE__); \
    } while (0)

END_DECLS
