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
 * Adesto P/N AT45DB641E
 *
 * 64-Mbit DataFlash (with Extra 2-Mbits), 1.7V Minimum SPI Serial
 * Flash Memory
 */

/**
 * Enable optional features.
 */
#define __AT45_HAS_HIGHFREQ__
#define __AT45_HAS_BUF2__
#define __AT45_HAS_SUSPEND__
