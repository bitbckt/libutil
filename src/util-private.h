#pragma once

#include <portable/macros.h>

BEGIN_DECLS

#define UNUSED(x) ((void)x)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

END_DECLS
