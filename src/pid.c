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

#include <portable/smath.h>
#include <util/log.h>
#include <util/pid.h>

#include "assert.h"
#include "util-private.h"
#include "xmalloc.h"

struct pid_t {
    int_fast32_t kp;       /* proportional gain (const) */
    int_fast32_t ki;       /* integral gain (const) */
    int_fast32_t kd;       /* derivative gain (const) */
    int_fast16_t sp;       /* sp from the previous iteration */
    int_fast16_t error;    /* error from the previous iteration */
    int_fast32_t integral; /* accumulated error */
};

UTIL_EXPORT struct pid_t *
pid_init(float kp, float ki, float kd, float hz)
{
    struct pid_t *pid;

    ASSERT(kp > 0 && kp < UINT8_MAX);
    ASSERT(ki > 0 && ki < UINT8_MAX);
    ASSERT(kd > 0 && kd < UINT8_MAX);

    pid = xmalloc(sizeof(struct pid_t));

    /* scale gains to avoid float computation in pid_update() */
    pid->kp = (int32_t)(kp * (UINT8_MAX + 1));
    pid->ki = (int32_t)((ki * hz) * (UINT8_MAX + 1));
    pid->kd = (int32_t)((kd / hz) * (UINT8_MAX + 1));
    pid->sp = INT16_C(0);
    pid->integral = INT32_C(0);
    pid->error = INT32_C(0);

    return pid;
}

UTIL_EXPORT int_fast16_t
pid_update(struct pid_t *pid, int_fast16_t pv, int_fast16_t sp)
{
    int_fast16_t error;
    int_fast16_t deriv;
    int_fast32_t control;
    int_fast64_t process;

    error = sp - pv;

    control = pid->kp * error;

    /*
     * reset accumulated error when on target to prevent integral
     * windup
     */
    if (error == 0) {
        pid->integral = 0;
    } else {
        pid->integral = sadd32(pid->integral, error);
    }

    deriv = sadd16(error - pid->error, -(sp - pid->sp));

    pid->error = error;
    pid->sp = sp;

    process = control + (pid->ki * pid->integral) + (pid->kd * deriv);

    /* remove scaling factor and clamp output to i16 */
    return CLAMP(process / UINT8_MAX, INT16_MAX, INT16_MIN);
}

UTIL_EXPORT void
pid_deinit(struct pid_t *pid)
{
    xfree(pid);
}
