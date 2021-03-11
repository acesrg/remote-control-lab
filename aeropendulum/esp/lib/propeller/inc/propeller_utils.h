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
#ifndef EXAMPLES_PROJECT_LIB_PROPELLER_INC_PROPELLER_UTILS_H_
#define EXAMPLES_PROJECT_LIB_PROPELLER_INC_PROPELLER_UTILS_H_
#include <stdint.h>

#include <retval.h>

/**
 * \brief   Initialize propeller driver.
 * \return  retval_t: exit status
 */
retval_t initialize_propeller(void);

/**
 * \brief   De-initialize propeller driver.
 * \return  retval_t: exit status
 */
retval_t deinitialize_propeller(void);

/**
 * \brief   Update propeller speed.
 * \argument    duty: uint16_t value, representing the duty
 *              percentage, assigned to the propeller driver.
 *              TODO(marcotti): limit the duty to MAX and MIN, and
 *              specify limits here.
 * \return  retval_t: exit status
 */
retval_t set_propeller_duty(uint16_t duty);

#endif /* EXAMPLES_PROJECT_LIB_PROPELLER_INC_PROPELLER_UTILS_H_ */
