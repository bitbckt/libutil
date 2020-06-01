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

#include <errno.h>
#include <util/at45.h>

/* initial chip */
#define __AT45DQ321__

#include "at45_ops.h"
#include "util-private.h"

/*
 * JEDEC Device ID byte 1 to density in Mbit.
 */
static const uint8_t _at45_density_codes[] = {0, 0, 0, 2, 4, 8, 16, 32, 64};

/* clock the opcode and addr into dev */
static void _at45_opaddr(const at45_t *dev, uint8_t opcode, uint_fast32_t addr,
                         at45_freq_t freq);

/* continuous read */
static ssize_t _at45_read(const at45_t *dev, const uint8_t *dst,
                          uint_fast32_t src, size_t size, uint8_t opcode,
                          at45_freq_t freq);

/* poll status register after a write, return !EPE */
static bool _at45_writeready(const at45_t *dev);

/* clock an erase opcode into dev and return _at45_writeready */
static bool _at45_erase(const at45_t *dev, uint_fast32_t addr, uint8_t opcode);

UTIL_EXPORT int
at45_init(at45_t *const dev)
{
    /* density code in the low 5 bits */
    uint8_t dmask = BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
    uint8_t idx;

    at45_deviceid(dev);
    at45_pagesize_read(dev);

    idx = dev->device[0] & dmask;

    if (idx >= sizeof(_at45_density_codes)) {
        return -EINVAL;
    }

    dev->density_mbit = _at45_density_codes[idx];

    if (dev->density_mbit == 0) {
        return -EINVAL;
    }

    return 0;
}

UTIL_EXPORT ssize_t
at45_read(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
          size_t size)
{
    return _at45_read(dev, dst, src, size, CONT_READ_HF0, AT45_CAR1);
}

UTIL_EXPORT ssize_t
at45_read_hf(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
             size_t size)
{
#ifdef __AT45_HAS_HIGHFREQ__
    return _at45_read(dev, dst, src, size, CONT_READ_HF1, AT45_CAR4);
#else
    UNUSED(dev);
    UNUSED(dst);
    UNUSED(src);
    UNUSED(size);

    return -EINVAL;
#endif /* !__AT45_HAS_HIGHFREQ__ */
}

UTIL_EXPORT ssize_t
at45_read_lf(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
             size_t size)
{
    return _at45_read(dev, dst, src, size, CONT_READ_LF, AT45_CAR2);
}

UTIL_EXPORT ssize_t
at45_read_lp(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
             size_t size)
{
    return _at45_read(dev, dst, src, size, CONT_READ_LPM, AT45_CAR3);
}

UTIL_EXPORT ssize_t
at45_readpage(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
              size_t size)
{
    return _at45_read(dev, dst, src, size, PAGE_READ, AT45_SCK);
}

UTIL_EXPORT ssize_t
at45_readbuf(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
             size_t size, bool buf1)
{
    uint8_t opcode;

    if (buf1) {
        opcode = BUF1_READ_HF;
    } else {
#ifdef __AT45_HAS_BUF2__
        opcode = BUF2_READ_HF;
#else
        return -EINVAL;
#endif /* !__AT45_HAS_BUF2__ */
    }

    return _at45_read(dev, dst, src, size, opcode, AT45_SCK);
}

UTIL_EXPORT ssize_t
at45_readbuf_lf(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
                size_t size, bool buf1)
{
    uint8_t opcode;

    if (buf1) {
        opcode = BUF1_READ_LF;
    } else {
#ifdef __AT45_HAS_BUF2__
        opcode = BUF2_READ_LF;
#else
        return -EINVAL;
#endif /* !__AT45_HAS_BUF2__ */
    }

    return _at45_read(dev, dst, src, size, opcode, AT45_CAR2);
}

UTIL_EXPORT int
at45_copy(const at45_t *const dev, uint_fast32_t src, bool buf1)
{
    uint8_t opcode;

    if (buf1) {
        opcode = PAGE_COPY_BUF1;
    } else {
#ifdef __AT45_HAS_BUF2__
        opcode = PAGE_COPY_BUF2;
#else
        return -EINVAL;
#endif /* !__AT45_HAS_BUF2__ */
    }

    dev->spi_select(true);

    _at45_opaddr(dev, opcode, src, AT45_SCK);

    dev->spi_select(false);

    return 0;
}

UTIL_EXPORT int
at45_compare(const at45_t *const dev, uint_fast32_t addr, bool buf1)
{
    uint8_t opcode;

    if (buf1) {
        opcode = PAGE_CMP_BUF1;
    } else {
#ifdef __AT45_HAS_BUF2__
        opcode = PAGE_CMP_BUF2;
#else
        return -EINVAL;
#endif /* !__AT45_HAS_BUF2__ */
    }

    dev->spi_select(true);

    _at45_opaddr(dev, opcode, addr, AT45_SCK);

    dev->spi_select(false);

    return 0;
}

UTIL_EXPORT bool
at45_cr(const at45_t *const dev)
{
    uint8_t byte;

    dev->spi_select(true);

    dev->spi_transfer(STATUS_READ, AT45_SCK);
    byte = dev->spi_transfer(UINT8_C(0), AT45_SCK);

    dev->spi_select(false);

    return !ISSET(byte, BYTE1_CR);
}

UTIL_EXPORT ssize_t
at45_write_erase(const at45_t *const dev, uint_fast32_t dst,
                 const uint8_t *const src, size_t size, bool buf1)
{
    uint8_t  opcode;
    uint8_t *cur = (uint8_t *)src;
    uint8_t *lim = (uint8_t *)src + size;

    if (buf1) {
        opcode = PAGE_PROG_BUF1_ERASE;
    } else {
#ifdef __AT45_HAS_BUF2__
        opcode = PAGE_PROG_BUF2_ERASE;
#else
        return -EINVAL;
#endif /* !__AT45_HAS_BUF2__ */
    }

    dev->spi_select(true);

    _at45_opaddr(dev, opcode, dst, AT45_SCK);

    /* begin clocking in data */
    while (cur != lim) {
        dev->spi_transfer(*cur, AT45_SCK);
        ++cur;
    }

    dev->spi_select(false);

    if (_at45_writeready(dev)) {
        return size;
    }

    return -1;
}

UTIL_EXPORT ssize_t
at45_write(const at45_t *const dev, uint_fast32_t dst,
           const uint8_t *const src, size_t size)
{
    uint8_t *cur = (uint8_t *)src;
    uint8_t *lim = (uint8_t *)src + size;

    dev->spi_select(true);

    _at45_opaddr(dev, PAGE_PROG, dst, AT45_SCK);

    /* begin clocking in data */
    while (cur != lim) {
        dev->spi_transfer(*cur, AT45_SCK);
        ++cur;
    }

    dev->spi_select(false);

    if (_at45_writeready(dev)) {
        return size;
    }

    return -1;
}

UTIL_EXPORT ssize_t
at45_writebuf(const at45_t *const dev, uint_fast32_t dst,
              const uint8_t *const src, size_t size, bool buf1)
{
    uint8_t  opcode;
    uint8_t *cur = (uint8_t *)src;
    uint8_t *lim = (uint8_t *)src + size;

    if (buf1) {
        opcode = BUF1_WRITE;
    } else {
#ifdef __AT45_HAS_BUF2__
        opcode = BUF2_WRITE;
#else
        return -EINVAL;
#endif /* !__AT45_HAS_BUF2__ */
    }

    dev->spi_select(true);

    _at45_opaddr(dev, opcode, dst, AT45_SCK);

    /* begin clocking in data */
    while (cur != lim) {
        dev->spi_transfer(*cur, AT45_SCK);
        ++cur;
    }

    dev->spi_select(false);

    if (_at45_writeready(dev)) {
        return size;
    }

    return -1;
}

UTIL_EXPORT int
at45_writeback(const at45_t *dev, uint_fast32_t dst, bool buf1, bool erase)
{
    uint8_t opcode;

    if (buf1 && erase) {
        opcode = BUF1_PAGE_PROG_ERASE;
    } else if (buf1) {
        opcode = BUF1_PAGE_PROG_NOERASE;
    } else {
#ifdef __AT45_HAS_BUF2__
        if (erase) {
            opcode = BUF2_PAGE_PROG_ERASE;
        } else {
            opcode = BUF1_PAGE_PROG_NOERASE;
        }
#else
        return -EINVAL;
#endif /* !__AT45_HAS_BUF2__ */
    }

    dev->spi_select(true);

    _at45_opaddr(dev, opcode, dst, AT45_SCK);

    dev->spi_select(false);

    return _at45_writeready(dev) ? 1 : -EIO;
}

UTIL_EXPORT bool
at45_erasepage(const at45_t *const dev, uint_fast32_t addr)
{
    return _at45_erase(dev, addr, PAGE_ERASE);
}

UTIL_EXPORT bool
at45_eraseblock(const at45_t *const dev, uint_fast32_t addr)
{
    return _at45_erase(dev, addr, BLOCK_ERASE);
}

UTIL_EXPORT bool
at45_erasesector(const at45_t *const dev, uint_fast32_t addr)
{
    return _at45_erase(dev, addr, SECTOR_ERASE);
}

UTIL_EXPORT bool
at45_erasechip(const at45_t *const dev)
{
    uint8_t opcode[] = CHIP_ERASE;
    int     i;
    int     limit;

    dev->spi_select(true);

    limit = sizeof(opcode);
    for (i = 0; i < limit; ++i) {
        dev->spi_transfer(opcode[i], AT45_SCK);
    }

    dev->spi_select(false);

    return _at45_writeready(dev);
}

UTIL_EXPORT bool
at45_ready(const at45_t *const dev)
{
    uint8_t byte;

    dev->spi_select(true);

    dev->spi_transfer(STATUS_READ, AT45_SCK);
    byte = dev->spi_transfer(UINT8_C(0), AT45_SCK);

    dev->spi_select(false);

    return ISSET(byte, BYTE1_READY);
}

UTIL_EXPORT bool
at45_pagesize(const at45_t *const dev)
{
    return dev->dataflash_pages;
}

UTIL_EXPORT bool
at45_pagesize_read(at45_t *const dev)
{
    uint8_t byte;

    dev->spi_select(true);

    dev->spi_transfer(STATUS_READ, AT45_SCK);
    byte = dev->spi_transfer(UINT8_C(0), AT45_SCK);

    dev->spi_select(false);

    dev->dataflash_pages = !ISSET(byte, BYTE1_PAGE_SIZE);

    return dev->dataflash_pages;
}

UTIL_EXPORT bool
at45_epe(const at45_t *const dev)
{
    uint8_t byte;

    dev->spi_select(true);

    dev->spi_transfer(STATUS_READ, AT45_SCK);

    /* EPE is in byte 2 */
    dev->spi_transfer(UINT8_C(0), AT45_SCK);
    byte = dev->spi_transfer(UINT8_C(0), AT45_SCK);

    dev->spi_select(false);

    return ISSET(byte, BYTE2_EPE);
}

UTIL_EXPORT void
at45_deviceid(at45_t *const dev)
{
    dev->spi_select(true);

    dev->spi_transfer(JEDEC_READ, AT45_SCK);

    dev->manufacturer = dev->spi_transfer(UINT8_C(0), AT45_SCK);
    dev->device[0] = dev->spi_transfer(UINT8_C(0), AT45_SCK);
    dev->device[1] = dev->spi_transfer(UINT8_C(0), AT45_SCK);

    dev->spi_select(false);
}

UTIL_EXPORT void
at45_deinit(at45_t *dev)
{
    UNUSED(dev);
}

static void
_at45_opaddr(const at45_t *const dev, uint8_t opcode, uint_fast32_t addr,
             at45_freq_t freq)
{
    dev->spi_transfer(opcode, freq);
    dev->spi_transfer((addr >> UINT16_WIDTH) & UINT8_MAX, freq);
    dev->spi_transfer((addr >> UINT8_WIDTH) & UINT8_MAX, freq);
    dev->spi_transfer(addr & UINT8_MAX, freq);
}

static ssize_t
_at45_read(const at45_t *const dev, const uint8_t *dst, uint_fast32_t src,
           size_t size, uint8_t opcode, at45_freq_t freq)
{
    uint8_t *cur = (uint8_t *)dst;
    uint8_t *lim = (uint8_t *)dst + size;

    dev->spi_select(true);

    _at45_opaddr(dev, opcode, src, freq);

    while (cur < lim) {
        *cur = dev->spi_transfer(UINT8_C(0), freq);
        ++cur;
    }

    dev->spi_select(false);

    return size;
}

static bool
_at45_writeready(const at45_t *const dev)
{
    uint8_t byte1 = UINT8_C(0);
    uint8_t byte2;

    /* TODO: support non-blocking wait? */
    while (!ISSET(byte1, BYTE1_READY)) {
        /* wait for ready */

        dev->spi_select(true);

        dev->spi_transfer(STATUS_READ, AT45_SCK);
        byte1 = dev->spi_transfer(UINT8_C(0), AT45_SCK);
        byte2 = dev->spi_transfer(UINT8_C(0), AT45_SCK);

        dev->spi_select(false);
    }

    return !ISSET(byte2, BYTE2_EPE);
}

static bool
_at45_erase(const at45_t *const dev, uint_fast32_t addr, uint8_t opcode)
{
    dev->spi_select(true);

    _at45_opaddr(dev, opcode, addr, AT45_SCK);

    dev->spi_select(false);

    return _at45_writeready(dev);
}
