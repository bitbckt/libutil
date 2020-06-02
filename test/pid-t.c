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

#include <portable/system.h>
#include <test/tap/basic.h>
#include <util/log.h>
#include <util/pid.h>

static void
test_bump(void)
{
    struct pid_t *pid = bmalloc(sizeof(struct pid_t));
    int           i;
    int16_t       cv;
    int16_t       mv;
    int16_t       sp;
    int           err;

    err = pid_init(pid, 0.1, 0.01, 0.1, 1.0);
    is_int(0, err, "pid init %i", err);

    /* start @ 0, step to 10 */
    cv = 0;
    sp = 10;

    for (i = 0; i < 45; i++) {
        mv = pid_update(pid, cv, sp);

        cv += mv;
        log_stdout("%i %i", cv, mv);
    }

    is_int(0, mv, "PID stable");
    is_int(sp, cv, "control stable");

    /* step to 100 */
    sp = 100;

    for (i = 0; i < 30; i++) {
        mv = pid_update(pid, cv, sp);

        cv += mv;
        log_stdout("%i %i", cv, mv);
    }

    is_int(0, mv, "PID stable");
    is_int(sp, cv, "control stable");

    /* step back to 10 */
    sp = 10;

    for (i = 0; i < 15; i++) {
        mv = pid_update(pid, cv, sp);

        cv += mv;
        log_stdout("%i %i", cv, mv);
    }

    is_int(0, mv, "PID stable");
    is_int(sp, cv, "control stable");

    pid_deinit(pid);
}

int
main(void)
{
    plan(7);

    test_bump();

    return EXIT_SUCCESS;
}
