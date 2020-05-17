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

void _assert(const char *cond, const char *file, int line, bool panic)
    __attribute__((nonnull));
void stacktrace(int skip);

/*
 * Wrappers for defining custom assert based on whether the macro
 * ASSERT_PANIC was defined at the moment ASSERT was called.
 */
#ifdef NDEBUG
#    define ASSERT(_x)
#    define NOT_REACHED()
#else /* !NDEBUG */
#    ifdef ASSERT_PANIC
#        define ASSERT(_x)                                  \
            do {                                            \
                if (!(_x)) {                                \
                    _assert(#_x, __FILE__, __LINE__, true); \
                }                                           \
            } while (0)

#        define NOT_REACHED() ASSERT(0)
#    else /* !ASSERT_PANIC */
#        define ASSERT(_x)                                   \
            do {                                             \
                if (!(_x)) {                                 \
                    _assert(#_x, __FILE__, __LINE__, false); \
                }                                            \
            } while (0)

#        define NOT_REACHED() ASSERT(0)
#    endif /* ASSERT_PANIC */
#endif     /* NDEBUG */

END_DECLS
