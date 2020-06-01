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

/**
 * SPI frequency domains.
 *
 * Refer to Section 18.4 ("AC Characteristics") in the datasheet for
 * the specific device for more information.
 */
typedef enum {
    AT45_SCK,  /* SCK frequency */
    AT45_CAR1, /* continuous read (high frequency) */
    AT45_CAR2, /* continuous read (low frequency) */
    AT45_CAR3, /* continuous read (low power */
    AT45_CAR4  /* continuous read (highest frequency) */
} at45_freq_t;

/**
 * SPI transfer function type.
 *
 * A transfer function should clock the provided input byte out on
 * MOSI, and return the bytes clocked in on MISO. It should _not_
 * manipulate slave-select.
 *
 * The clock rate must conform the frequency domain of the device and
 * operation in question. Refer to Section 18.4 ("AC Characteristics")
 * in the datasheet for the specific device for more information.
 */
typedef uint8_t (*at45_transfer_fn)(uint8_t, at45_freq_t);

/**
 * SPI slave-select function type.
 *
 * A slave-select function should drive the select pin according to
 * the given bool, where `true` means "enabled" and `false` means
 * "disabled".
 */
typedef void (*at45_select_fn)(bool);

/**
 * Driver for the Adesto DataFlash SPI memory devices.
 *
 * Website:
 *
 *    https://www.adestotech.com/products/data-flash/
 *
 * Addressing:
 *
 *    Memory addresses for AT45 devices are 24 bits wide. The low bits
 *    (B) are used for buffer offset addressing, and the remaining
 *    high bits (P) are used for page addressing.
 *
 *    DataFlash pages use 10 buffer offset bits, and up to 14
 *    page address bits, depending upon the device density.
 *
 *    "Power-of-2" pages use 9 buffer offset bits, and up to 13 page
 *    address bits, depending upon the device density. The high bit is
 *    always reserved.
 *
 *    DataFlash:
 *
 *     [ P P P P P P P P | P P P P P P B B | B B B B B B B B ]
 *      23                                                 0
 *
 *    Power-of-2:
 *     [ X P P P P P P P | P P P P P P P B | B B B B B B B B ]
 *      23                                                 0
 *
 */
typedef struct {
    uint8_t density_mbit;    /* device density in Mbits (const) */
    bool    dataflash_pages; /* DataFlash pages enabled */

    uint8_t manufacturer; /* JEDEC manufacturer ID byte (const) */
    uint8_t device[2];    /* JEDEC device ID bytes (const) */

    at45_transfer_fn spi_transfer; /* SPI transfer function */
    at45_select_fn   spi_select;   /* SPI slave-select */
} at45_t;

/**
 * Initialize a connection to a memory device `dev` using SPI.
 *
 * Assumes that SPI is initialized to either SPI Mode 0 or 3, MSB
 * first.
 *
 * Security Register, Dual-I/O, Quad-I/O, Suspend/Resume, and Sector
 * Protection/Lockdown are not currently supported.
 *
 * Returns non-zero if initialization has failed, zero otherwise.
 */
int at45_init(at45_t *dev) __attribute__((nonnull));

/**
 * Reads up to `size` bytes from a main memory page on `dev` at
 * address `src` into `dst` at high frequency.
 *
 * If the read reaches the end of a page, the read will wrap around to
 * the beginning of the page. If the read reaches the end of main
 * memory, the read will wrap around to the beginning of the first
 * page in memory.
 *
 * Returns the number of bytes read successfully, or a negative errno.
 */
ssize_t at45_read(const at45_t *dev, const uint8_t *dst, uint_fast32_t src,
                  size_t size) __attribute__((nonnull(1, 2)));

/**
 * Reads up to `size` bytes from a main memory page on `dev` at
 * address `src` into `dst` at the highest frequency.
 *
 * If the read reaches the end of a page, the read will wrap around to
 * the beginning of the page. If the read reaches the end of main
 * memory, the read will wrap around to the beginning of the first
 * page in memory.
 *
 * Returns the number of bytes read successfully, or a negative errno.
 */
ssize_t at45_read_hf(const at45_t *dev, const uint8_t *dst, uint_fast32_t src,
                     size_t size) __attribute__((nonnull(1, 2)));

/**
 * Reads up to `size` bytes from a main memory page on `dev` at
 * address `src` into `dst` at low frequency.
 *
 * If the read reaches the end of a page, the read will wrap around to
 * the beginning of the page. If the read reaches the end of main
 * memory, the read will wrap around to the beginning of the first
 * page in memory.
 *
 * Returns the number of bytes read successfully, or a negative errno.
 */
ssize_t at45_read_lf(const at45_t *dev, const uint8_t *dst, uint_fast32_t src,
                     size_t size) __attribute__((nonnull(1, 2)));

/**
 * Reads up to `size` bytes from a main memory page on `dev` at
 * address `src` into `dst` at low power.
 *
 * If the read reaches the end of a page, the read will wrap around to
 * the beginning of the page. If the read reaches the end of main
 * memory, the read will wrap around to the beginning of the first
 * page in memory.
 *
 * Returns the number of bytes read successfully, or a negative errno.
 */
ssize_t at45_read_lp(const at45_t *dev, const uint8_t *dst, uint_fast32_t src,
                     size_t size) __attribute__((nonnull(1, 2)));

/**
 * Reads up to `size` bytes from a main memory page on `dev` at
 * address `src` into `dst`.
 *
 * The main memory read bypasses the device buffers and leaves them
 * unmodified.
 *
 * If the read reaches the end of a page, the read will wrap around to
 * the beginning of the page.
 *
 * Returns the number of bytes read successfully, or a negative errno.
 */
ssize_t at45_readpage(const at45_t *dev, const uint8_t *dst, uint_fast32_t src,
                      size_t size) __attribute__((nonnull(1, 2)));

/**
 * Reads up to `size` bytes from a buffer on device `dev` into `buf`
 * at high frequency.
 *
 * If `buf1` is true, Buffer 1 will be read, otherwise Buffer 2 will
 * be read.
 *
 * Bytes are read starting at the buffer offset in the given address,
 * and continue reading until size bytes have been read.
 *
 * Returns the number of bytes read successfully, or a negative errno.
 */
ssize_t at45_readbuf(const at45_t *dev, const uint8_t *dst, uint_fast32_t src,
                     size_t size, bool buf1) __attribute__((nonnull(1, 2)));

/**
 * Reads up to `size` bytes from a buffer on device `dev` into `buf`
 * at low frequency.
 *
 * If `buf1` is true, Buffer 1 will be read, otherwise Buffer 2 will
 * be read.
 *
 * Returns the number of bytes read successfully, or a negative errno.
 */
ssize_t at45_readbuf_lf(const at45_t *dev, const uint8_t *dst,
                        uint_fast32_t src, size_t size, bool buf1)
    __attribute__((nonnull(1, 2)));

/**
 * Copies a page of data from the main memory address `src` to a buffer.
 *
 * If `buf1` is true, Buffer 1 receives the data, otherwise Buffer
 * 2 receives it.
 *
 * This function does not block during the copy. at45_ready() will
 * return false while the copy occurs, and true once the operation
 * completes.
 *
 * Returns 0 if the copy was issued successfully, and non-zero if
 * there was an error.
 */
int at45_copy(const at45_t *dev, uint_fast32_t src, bool buf1)
    __attribute__((nonnull));

/**
 * Compares a page of data from the main memory address `addr` to a buffer.
 *
 * * If `buf1` is true, Buffer 1 is compared, otherwise Buffer
 * 2 is.
 *
 * at45_ready() will return false during the comparison, and true once
 * the result is available in at45_cr().
 *
 * Returns 0 if the compare was issued successfully, and non-zero if
 * there was an error.
 */
int at45_compare(const at45_t *dev, uint_fast32_t addr, bool buf1)
    __attribute__((nonnull));

/**
 * Returns the result of the last comparison.
 *
 * See at45_compare().
 */
bool at45_cr(const at45_t *dev) __attribute__((nonnull));

/**
 * Writes `size` bytes from `src` through a buffer on `dev` to the
 * page address `dst`. If `buf1` is true, Buffer 1 is used, otherwise
 * Buffer 2 is used.
 *
 * If the end of the buffer reached, writes will wrap around to the
 * beginning of the buffer.
 *
 * After data is transferred to the indicated buffer, the addressed
 * page is erased, and the buffer's contents are written back to the
 * page.
 *
 * Returns -1 if the write failed, otherwise returns the number of
 * bytes written.
 */
ssize_t at45_write_erase(const at45_t *dev, uint_fast32_t dst,
                         const uint8_t *src, size_t size, bool buf1)
    __attribute__((nonnull(1, 3)));

/**
 * Writes `size` bytes from `src` through Buffer 1 on `dev` to the
 * page address `dst`.
 *
 * After data is transferred to the indicated buffer, the buffer's
 * contents are written back to the page starting at page byte 0.
 *
 * If the end of the buffer reached, writes will wrap around to the
 * beginning of the buffer.
 *
 * If the size of the buffer is smaller than the configured page size,
 * the remaining bytes in the page are left unmodified.
 *
 * Returns -1 if the write failed, otherwise returns the number of
 * bytes written.
 */
ssize_t at45_write(const at45_t *dev, uint_fast32_t dst, const uint8_t *src,
                   size_t size) __attribute__((nonnull(1, 3)));

/**
 * Writes `size` bytes from `src` into a buffer on `dev`. If `buf1` is
 * true, Buffer 1 is used, otherwise Buffer 2 is used.
 *
 * Bytes are written starting at the buffer offset in the given `dst`
 * address, and continue writing until size bytes have been written.
 *
 * If the end of the buffer is reached, writes will wrap around to the
 * beginning of the buffer.
 *
 * Returns -1 if the write failed, otherwise returns the number of
 * bytes written.
 */
ssize_t at45_writebuf(const at45_t *dev, uint_fast32_t dst, const uint8_t *src,
                      size_t size, bool buf1) __attribute__((nonnull(1, 3)));

/**
 * Write-back from a buffer on `dev` to main memory at the page
 * address `dst`.
 *
 * If `buf1` is true, Buffer 1 is written back to main memory, otherwise
 * Buffer 2 is written back.
 *
 * If `erase` is true, the selected page will be erased prior to the
 * program cycle.
 *
 * Returns a negative errno if the write failed, otherwise returns 1.
 */
int at45_writeback(const at45_t *dev, uint_fast32_t dst, bool buf1, bool erase)
    __attribute__((nonnull));

/**
 * Erases a page on the device `dev` at the page address `addr`.
 *
 * Returns true if the erase succeeded, or false if an error occurred.
 */
bool at45_erasepage(const at45_t *dev, uint_fast32_t addr)
    __attribute__((nonnull));

/**
 * Erases a block of 8 pages on the device `dev` at the address
 * `addr`. This is more efficient than issuing 8 at45_erasepage()
 * calls.
 *
 * Block addressing uses the page address bits to address blocks
 * numbered 0 - 1023:
 *
 *    DataFlash:
 *
 *     [ X P P P P P P P | P P P P P P B B | B B B B B B B B ]
 *      23                                                 0
 *
 *    Power-of-2:
 *     [ X X P P P P P P | P P P P P P P B | B B B B B B B B ]
 *      23                                                 0
 *
 * Returns true if the erase succeeded, or false if an error occurred.
 */
bool at45_eraseblock(const at45_t *dev, uint_fast32_t addr)
    __attribute__((nonnull));

/**
 * Erases a sector of pages on the device `dev`, at the address `addr`.
 *
 * Sector addressing uses the page address bits to address
 * sectors. Depending upon the device density, there may be as few as
 * 9 sectors and as many as 65.
 *
 * Refer to Sections 3 ("Memory Array") and 6.10 ("Sector Erase") for
 * the specific device for more information.
 *
 * Returns true if the erase succeeded, or false if an error occurred.
 */
bool at45_erasesector(const at45_t *dev, uint_fast32_t addr)
    __attribute__((nonnull));

/**
 * Erases the entire main memory at one time.
 *
 * Chip erase will not affect protected or locked down sectors.
 *
 * Returns true if the erase succeeded, or false if an error occurred.
 */
bool at45_erasechip(const at45_t *dev) __attribute__((nonnull));

/**
 * Checks the ready status of the device `dev`.
 *
 * Returns true if the device is ready, false if it is busy.
 */
bool at45_ready(const at45_t *dev) __attribute__((nonnull));

/**
 * Reads the page size configuration from the device `dev`.
 *
 * Returns true if `dev` is configured for standard DataFlash pages,
 * and false if configured for "power-of-2" pages.
 *
 * WARNING:
 *     This does _NOT_ refresh configuration from the device. It may
 *     be out of date. See at45_pagesize_read().
 */
bool at45_pagesize(const at45_t *dev) __attribute__((nonnull));

/**
 * Reads the page size configuration from the status register on the
 * device `dev`, and sets the appropriate fields in `dev`.
 *
 * Returns true if `dev` is configured for standard DataFlash pages,
 * and false if configured for "power-of-2" pages.
 */
bool at45_pagesize_read(at45_t *dev) __attribute__((nonnull));

/**
 * Reads the status register on the device `dev` and returns whether
 * an error occurred during the previous program/erase cycle.
 */
bool at45_epe(const at45_t *dev) __attribute__((nonnull));

/**
 * Reads the JEDEC-defined Manufacturer ID, vendor-specific Device ID
 * from the device `dev` and sets the appropriate fields in `dev`.
 *
 * WARNING:
 *     "Since not all Flash devices are capable of operating at very
 *     high clock frequencies, applications should be designed to read
 *     the identification information from the devices at a reasonably
 *     low clock frequency to ensure that all devices to be used in
 *     the application can be identified properly."
 */
void at45_deviceid(at45_t *dev) __attribute__((nonnull));

/**
 * Returns resources allocated for `dev` during at45_init().
 */
void at45_deinit(at45_t *dev) __attribute__((nonnull));

END_DECLS
