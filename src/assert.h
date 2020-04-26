#pragma once

#include <portable/macros.h>
#include <portable/stdbool.h>

BEGIN_DECLS

void _assert(const char *cond, const char *file, int line, bool panic);
void stacktrace(int skip);

/*
 * Wrappers for defining custom assert based on whether the macro
 * ASSERT_PANIC was defined at the moment ASSERT was called.
 */
#ifdef ASSERTS
#    ifdef ASSERT_PANIC
#        define ASSERT(_x) do {                                     \
                if (!(_x)) {                                        \
                    _assert(#_x, __FILE__, __LINE__, true);         \
                }                                                   \
            } while (0)

#        define NOT_REACHED() ASSERT(0)
#    else /* !ASSERT_PANIC */
#        define ASSERT(_x) do {                                  \
                if (!(_x)) {                                     \
                    _assert(#_x, __FILE__, __LINE__, false);     \
                }                                                \
            } while (0)

#        define NOT_REACHED() ASSERT(0)
#    endif /* ASSERT_PANIC */
#else /* !ASSERTS */
#    define ASSERT(_x)
#    define NOT_REACHED()
#endif /* ASSERTS */

END_DECLS
