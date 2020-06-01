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

#if defined(__AT45DB021E__)
#    include "at45db021e.h"
#    elsif defined(__AT45DB041E__)
#    include "at45db041e.h"
#    elsif defined(__AT45DB081E__)
#    include "at45db081e.h"
#    elsif defined(__AT45DB161E__)
#    include "at45db161e.h"
#    elsif defined(__AT45DQ321__)
#    include "at45dq321.h"
#    elsif defined(__AT45DB641E__)
#    include "at45db641e.h"
#endif

BEGIN_DECLS

/**
 * Read Commands
 */

/* main memory page read */
#define PAGE_READ 0xD2

/* continuous array read (low power mode) */
#define CONT_READ_LPM 0x01

/* continuous array read (low frequency) */
#define CONT_READ_LF 0x03

/* continuous array read (high frequency) */
#define CONT_READ_HF0 0x0B

/* buffer 1 read (low frequency) */
#define BUF1_READ_LF 0xD1

/* buffer 1 read (high frequency) */
#define BUF1_READ_HF 0xD4

/**
 * Program and Erase Commands
 */

/* buffer 1 write */
#define BUF1_WRITE 0x84

/* buffer 1 to main memory page program with built-in erase */
#define BUF1_PAGE_PROG_ERASE 0x83

/* buffer 1 to main memory page program without built-in erase */
#define BUF1_PAGE_PROG_NOERASE 0x88

/* main memory page program through buffer 1 with built-in erase */
#define PAGE_PROG_BUF1_ERASE 0x82

/* main memory page program through buffer 1 without built-in erase */
#define PAGE_PROG 0x02

/* page erase */
#define PAGE_ERASE 0x81

/* block erase */
#define BLOCK_ERASE 0x50

/* sector erase */
#define SECTOR_ERASE 0x7C

/* chip erase */
#define CHIP_ERASE             \
    {                          \
        0xC7, 0x94, 0x80, 0x9A \
    }

/**
 * Protection and Security Commands
 */

/* enable sector protection */
#define SECTOR_PROTECT_ENABLE  \
    {                          \
        0x3D, 0x2A, 0x7F, 0xA9 \
    }

/* disable sector protection */
#define SECTOR_PROTECT_DISABLE \
    {                          \
        0x3D, 0x2A, 0x7F, 0x9A \
    }

/* erase sector protection register */
#define SECTOR_PROTECT_ERASE   \
    {                          \
        0x3D, 0x2A, 0x7F, 0xCF \
    }

/* program sector protection register */
#define SECTOR_PROTECT_PROG    \
    {                          \
        0x3D, 0x2A, 0x7F, 0xFC \
    }

/* read sector protection register */
#define SECTOR_PROTECT_READ 0x32

/* sector lockdown */
#define SECTOR_LOCKDOWN        \
    {                          \
        0x3D, 0x2A, 0x7F, 0x30 \
    }

/* read sector lockdown register */
#define SECTOR_LOCKDOWN_READ 0x35

/* freeze sector lockdown */
#define SECTOR_LOCKDOWN_FREEZE \
    {                          \
        0x34, 0x55, 0xAA, 0x40 \
    }

/* program security register */
#define SECURITY_PROG          \
    {                          \
        0x9B, 0x00, 0x00, 0x00 \
    }

/* read security register */
#define SECURITY_READ 0x77

/**
 * Additional Commands
 */

/* main memory page to buffer 1 transfer */
#define PAGE_COPY_BUF1 0x53

/* main memory page to buffer 1 compare */
#define PAGE_CMP_BUF1 0x60

/* auto page rewrite through buffer 1 */
#define PAGE_REWRITE_BUF1 0x58

/* deep power down */
#define POWER_DOWN_DEEP 0xB9

/* resume from deep power down */
#define POWER_RESUME_DEEP 0xAB

/* utra-deep power down */
#define POWER_DOWN_ULTRA 0x79

/* read status register */
#define STATUS_READ 0xD7

/* manufacturer and device ID read */
#define JEDEC_READ 0x9F

/* configure "power of 2" page size */
#define POWER2_PAGES           \
    {                          \
        0x3D, 0x2A, 0x80, 0xA6 \
    }

/* configure standard DataFlash page size */
#define DATAFLASH_PAGES        \
    {                          \
        0x3D, 0x2A, 0x80, 0xA7 \
    }

/* soft reset */
#define RESET                  \
    {                          \
        0xF0, 0x00, 0x00, 0x00 \
    }

/**
 * Status Register Flags
 */

/* ready flag: busy = 0, ready = 1 */
#define BYTE1_READY BIT7

/* compare result: match = 0, miss = 1 */
#define BYTE1_CR BIT6

/* sector protection: disabled = 0, enabled = 1 */
#define BYTE1_PROTECT BIT1

/* page size: dataflash = 0, power2 = 1 */
#define BYTE1_PAGE_SIZE BIT0

/* ready flag: busy = 0, ready = 1 */
#define BTYE2_READY BIT7

/* erase/program error: success = 0, error = 1 */
#define BYTE2_EPE BIT5

/* sector lockdown: disabled = 0, enabled = 1 */
#define BYTE2_SLE BIT3

/**
 * Optional Features
 */

#ifdef __AT45_HAS_HIGHFREQ__

/* continuous array read (highest frequency) */
#    define CONT_READ_HF1 0x1B

#endif /* __AT45_HAS_HIGHFREQ__ */

#ifdef __AT45_HAS_BUF2__

/* buffer 2 read (low frequency) */
#    define BUF2_READ_LF 0xD3

/* buffer 2 read (high frequency) */
#    define BUF2_READ_HF 0xD6

/* buffer 2 write */
#    define BUF2_WRITE 0x87

/* buffer 2 to main memory page program with built-in erase */
#    define BUF2_PAGE_PROG_ERASE 0x86

/* buffer 2 to main memory page program without built-in erase */
#    define BUF2_PAGE_PROG_NOERASE 0x89

/* main memory page program through buffer 2 with built-in erase */
#    define PAGE_PROG_BUF2_ERASE 0x85

/* main memory page to buffer 2 transfer */
#    define PAGE_COPY_BUF2 0x55

/* main memory page to buffer 2 compare */
#    define PAGE_CMP_BUF2 0x61

/* auto page rewrite through buffer 2 */
#    define PAGE_REWRITE_BUF2 0x59

#endif /* __AT45_HAS_BUF2__ */

#ifdef __AT45_HAS_DUALIO__

/* dual-output array read */
#    define DUAL_READ 0x3B

/* buffer 1 dual-write */
#    define BUF1_DUAL_WRITE 0x24

#    ifdef __AT45_HAS_BUF2__
/* buffer 2 dual-write */
#        define BUF2_DUAL_WRITE 0x87

#    endif /* __AT45_HAS_BUF2__ */

#endif /* __AT45_HAS_DUALIO__ */

#ifdef __AT45_HASQUADIO__

/* quad-output array read */
#    define QUAD_READ 0x6B

/* buffer 1 quad-write */
#    define BUF1_QUAD_WRITE 0x44

#    ifdef __AT45_HAS_BUF2__
/* buffer 2 quad-write */
#        define BUF2_QUAD_WRITE 0x47
#    endif /* __AT45_HAS_BUF2__ */

/* quad enable */
#    define QUAD_ENABLE            \
        {                          \
            0x3D, 0x2A, 0x81, 0x66 \
        }

/* quad disable */
#    define QUAD_DISABLE           \
        {                          \
            0x3D, 0x2A, 0x81, 0x67 \
        }

/* read configuration register */
#    define CONFIG_READ 0x3F

#endif /* __AT45_HASQUADIO__ */

#ifdef __AT45_HAS_SUSPEND__

/* program/erase suspend */
#    define PROG_SUSPEND 0xB0

/* program/erase resume */
#    define PROG_RESUME 0xD0

/* program suspend status (buffer 2): none = 0, suspended = 1 */
#    define BYTE2_PS2 BIT2

/* program suspend status (buffer 1): none = 0, suspended = 1 */
#    define BYTE2_PS1 BIT1

/* erase suspend status: none = 0, suspended = 1 */
#    define BYTE2_ES BIT0

#endif /* __AT45_HAS_SUSPEND__ */

END_DECLS
