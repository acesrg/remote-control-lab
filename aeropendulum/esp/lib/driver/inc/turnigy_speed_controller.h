/*
 * Copyright 2021 Marco Miretti.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
/**
 * \file turnigy_speed_controller.h
 * \todo    A method is missing, that allows an user of this lib,
 *          to convert from a human-readable duty into the duty
 *          that turnigy speed controller uses.
 * */
#ifndef EXAMPLES_PROJECT_LIB_DRIVER_INC_TURNIGY_SPEED_CONTROLLER_H_
#define EXAMPLES_PROJECT_LIB_DRIVER_INC_TURNIGY_SPEED_CONTROLLER_H_
#include <stdint.h>

#include <retval.h>

/**
 * \brief   Initialize the turnigy driver.
 *
 * Configures the pin and sets the initialization value to the PWM.
 *
 * \note    After calling this method, the PWM duty shouldn't be
 *          altered for at least two seconds.
 *
 * \todo    Shouldn't the init sequence require a provided config?
 *
 * \return retval_t: error code.
 */
retval_t turnigy_speed_controller_init_sequence(void);

/**
 * \brief    Sets PWM duty to it's power-off value.
 *
 * \return retval_t: error code.
 */
retval_t turnigy_speed_controller_deinit_sequence(void);

/**
 * \brief   Updates turnigy drivers PWM's duty cycle.
 * This wrapper sets the duty for the PWM pin.
 *
 * \param   duty: A value in the range from 0x1C2A (0%) to 0x3333 (100%)
 *          that will set the duty on the driver. Is very important to
 *          note that the value range is NOT sanity checked.
 *
 * \return retval_t: error code.
 *
 * \note    The turnigy speed controller does not care much about duty,
 *          is all about duration of the active pulse, but not relative
 *          to the full square wave duration.
 *          This controller interprets the active pulse of a PWM signal
 *          with frequencies from 50 Hz up to 400 Hz. The range of the
 *          active part of the signal should be between 1 ms and 2 ms.
 *          The speed acts like this:
 *              - 1.0 ms: turn the motor off.
 *              - 1.5 ms: motor at half speed.
 *              - 2.0 ms: motor at full speed.
 *          It is a pretty standard behaviour for ESCs (electronic speed
 *          controllers).
 *
 * \todo    Should sanity-check input duty.
 */
retval_t turnigy_speed_controller_update_pwm_duty(uint16_t duty);
#endif /* EXAMPLES_PROJECT_LIB_DRIVER_INC_TURNIGY_SPEED_CONTROLLER_H_ */
