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
#include <portable/system.h>
#include <test/tap/basic.h>
#include <util/buffer.h>

/*
 * Basic operation of a dbuf.
 */
static void
test_basic(void)
{
    struct dbuf *dbuf;
    uint8_t byte = 'a';

    dbuf = dbuf_init(1);

    ok(dbuf_put(dbuf, byte), "put");
    ok(dbuf_get(dbuf, &byte), "get");
    ok(byte == 'a', "result");

    dbuf_deinit(dbuf);
}

/*
 * Test reaching write capacity with a slow reader.
 */
static void
test_capacity(void)
{
    struct dbuf *dbuf;
    uint8_t byte = 'a';

    dbuf = dbuf_init(26);

    while (byte < '{') {
        ok(dbuf_put(dbuf, byte), "put %c", byte);
        byte++;
    }

    ok(!dbuf_put(dbuf, byte), "at capacity");
    ok(dbuf_get(dbuf, &byte), "get one byte");
    ok(byte == 'a', "read the first letter");

    while (byte < '{') {
        ok(dbuf_put(dbuf, byte), "put %c", byte);
        byte++;
    }

    ok(!dbuf_put(dbuf, byte), "at capacity");

    dbuf_deinit(dbuf);
}

/*
 * Test no bytes are returned if none were written.
 */
static void
test_readable(void)
{
    struct dbuf *dbuf;
    uint8_t byte = 'a';

    dbuf = dbuf_init(1);

    ok(!dbuf_get(dbuf, &byte), "unreadable");
    ok(byte == 'a', "unmodified");

    ok(dbuf_put(dbuf, 'z'), "put one byte");
    ok(dbuf_get(dbuf, &byte), "readable");
    ok(byte == 'z', "modified");

    ok(!dbuf_get(dbuf, &byte), "unreadable");

    dbuf_deinit(dbuf);
}

int
main(void)
{
    plan_lazy();

    test_basic();
    test_capacity();
    test_readable();

    return EXIT_SUCCESS;
}
