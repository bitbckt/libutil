#include <portable/system.h>

#include "assert.h"
#include "util-private.h"

void
_assert(const char *cond, const char *file, int line, int panic)
{
    /* TODO: debug log */
    UNUSED(cond);
    UNUSED(file);
    UNUSED(line);
    
    if (panic) {
        abort();
    }
}
