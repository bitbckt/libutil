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
 * 16-bit proportional-integral-derivative (PID) controller.
 *
 * A PID controller continuously calculates an error value as the
 * difference between a desired setpoint (SP) and a measured process
 * variable (PV) and returns a correction factor.
 */
struct pid_t;

/**
 * Initialize a PID controller with the given gains and operating
 * frequency in Hz.
 *
 * All gain parameters must be positive values between [0.0, 255.0).
 */
struct pid_t *pid_init(float kp, float ki, float kd, float hz);

/**
 * Update a PID controller with a new process variable (PV), and
 * setpoint (SP).
 *
 * Returns a correction factor to be applied to the process under
 * control.
 */
int_fast16_t pid_update(struct pid_t *pid, int_fast16_t pv, int_fast16_t sp);

/**
 * Release resources allocated in pid_init().
 */
void pid_deinit(struct pid_t *pid);

END_DECLS
