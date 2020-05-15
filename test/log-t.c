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

#include <config.h>
#include <portable/macros.h>
#include <portable/system.h>
#include <test/tap/basic.h>
#include <test/tap/process.h>
#include <util/log.h>

static void
test_loggable(void)
{
    /* clang-format off */
    unsigned long i;
    int           levels[] = {LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR,
                              LOG_WARN, LOG_NOTICE, LOG_INFO, LOG_DEBUG};
    /* clang-format on */

    ok(log_init(LOG_EMERG, NULL), "level = EMERG");

    ok(log_loggable(LOG_EMERG), "msg @ EMERG");

    for (i = 1; i < ARRAY_SIZE(levels) - 1; i++) {
        ok(!log_loggable(i), "msg @ %li", i);
    }

    log_deinit();

    ok(log_init(LOG_DEBUG, NULL), "level = DEBUG");

    for (i = 0; i < ARRAY_SIZE(levels); i++) {
        ok(log_loggable(i), "msg @ %li", i);
    }

    log_deinit();
}

static void
test_out(void *data)
{
    (void)data; /* prevent -Wunused */

    log_stdout("stdout");
    log_stderr("stderr");
}

static void
test_output(void)
{
    char *dir = test_tmpdir();
    char *file = malloc(strlen(dir) + 4); /* dir + / + "log" + NUL */

    strcpy(file, dir);
    strcat(file, "/log");

    unlink(file); /* just in case; result doesn't matter */

    ok(log_init(LOG_INFO, file), "output file %s", file);

    log_info("info");
    log_notice("notice");
    log_warn("warn");
    log_error("error");
    log_crit("critical");
    log_alert("alert");
    log_emerg("emergency");

    is_function_output(test_out, NULL, EXIT_SUCCESS, "stdout\nstderr\n",
                       "standard output test");

    log_deinit();

    is_int(0, unlink(file), "unlink %s", file);

    free(file);
    test_tmpdir_free(dir);
}

int
main(void)
{
    plan_lazy();

    test_loggable();
    test_output();

    return EXIT_SUCCESS;
}
