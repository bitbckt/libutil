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
#include <portable/stdbool.h>
#include <portable/system.h>

BEGIN_DECLS

/**
 * Double buffer for ISR-safe I/O.
 *
 * When dbuf_get() meets the end of the read buffer, it swaps buffers.
 *
 * If dbuf_put() reaches the end of its buffer, it will refuse further
 * writes until dbuf_get() swaps buffers.
 */
struct dbuf;

/**
 * Allocates a dbuf with capacity in bytes given by size.
 *
 * NOTE: allocates 2 * size bytes plus administrative overhead.
 */
struct dbuf *dbuf_init(size_t size) __attribute__((warn_unused_result));

/**
 * Copies the first readable byte of dbuf into byte.
 *
 * Returns true if there was a readable byte, false otherwise.
 *
 * If no bytes were readable, byte is not modified.
 */
bool dbuf_get(struct dbuf *dbuf, uint8_t *byte) __attribute__((nonnull));

/**
 * Puts a byte in dbuf. Returns true if the byte was written, false if
 * dbuf has no writable capacity.
 */
bool dbuf_put(struct dbuf *dbuf, uint8_t byte) __attribute__((nonnull));

/**
 * Frees memory allocated for dbuf during dbuf_init().
 */
void dbuf_deinit(struct dbuf *dbuf) __attribute__((nonnull));

END_DECLS
