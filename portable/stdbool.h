#pragma once

#include <config.h>

#if HAVE_STDBOOL_H
#    include <stdbool.h>
#else
#    if HAVE__BOOL
#        define bool _Bool
#    else
#        ifdef __cplusplus
typedef bool _Bool;
#        elif _WIN32
#            include <windef.h>
#            define bool BOOL
#        else
typedef unsigned char _Bool;
#            define bool _Bool
#        endif
#    endif
#    define false 0
#    define true 1
#    define __bool_true_false_are_defined 1
#endif
