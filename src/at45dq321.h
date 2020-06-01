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

/**
 * Adesto P/N AT45DQ321
 *
 * 32-Mbit DataFlash (with Extra 1-Mbits), 2.3V Minimum SPI Serial
 * Flash Memory with Dual-I/O and Quad-I/O Support
 */

/**
 * Enable optional features.
 */
#define __AT45_HAS_HIGHFREQ__
#define __AT45_HAS_BUF2__
#define __AT45_HAS_DUALIO__
#define __AT45_HAS_QUADIO__
#define __AT45_HAS_SUSPEND__
