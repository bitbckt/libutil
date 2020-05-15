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
