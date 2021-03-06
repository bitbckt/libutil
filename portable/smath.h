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
#include <portable/system.h>

BEGIN_DECLS

#define MAX(_x, _y) (((_x) > (_y)) ? (_x) : (_y))
#define MIN(_x, _y) (((_x) < (_y)) ? (_x) : (_y))
#define CLAMP(_x, _min, _max) (MIN((_max), MAX((_x), (_min))))

/**
 * Saturated addition of two 16-bit signed integers.
 */
static int_fast16_t
sadd16(int_fast16_t a, int_fast16_t b)
{
    return CLAMP((int_fast32_t)a + b, INT16_MIN, INT16_MAX);
}

/**
 * Saturated addition of two 32-bit signed integers.
 */
static int_fast32_t
sadd32(int_fast32_t a, int_fast32_t b)
{
    return CLAMP((int_fast64_t)a + b, INT32_MIN, INT32_MAX);
}

END_DECLS
