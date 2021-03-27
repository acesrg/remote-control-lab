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
/** \file propeller_utils.h */
#ifndef EXAMPLES_PROJECT_LIB_PROPELLER_INC_PROPELLER_UTILS_H_
#define EXAMPLES_PROJECT_LIB_PROPELLER_INC_PROPELLER_UTILS_H_
#include <stdint.h>

#include <retval.h>

/**
 * \brief   Initialize the propeller.
 *
 * This wrapper calls the specific propeller initialization function
 * that will depend on the driver.
 *
 * \todo    Shouldn't the init sequence require a provided "pin" arg?
 *
 * \return retval_t: error code.
 */
retval_t initialize_propeller(void);

/**
 * \brief   Deinitialize the propeller.
 *
 * This wrapper calls the specific propeller deinitialization function
 * that will depend on the driver.
 *
 * \return retval_t: error code.
 */
retval_t deinitialize_propeller(void);

/**
 * \brief   Set a duty value for the propeller.
 *
 * This wrapper sets a duty (therefore changing the speed) in the
 * propeller driver.
 *
 * \param   duty: A value in the range from 0x1C2A (0%) to 0x3333 (100%)
 *          that will set the duty on the driver. Is very important to
 *          note that the value range is NOT sanity checked.
 *
 * \todo    This function should take a float from 0 to 100 representing
 *          the duty percentage, and not a cryptic hex number with
 *          who-knows-what-the-hell-it-means limits.
 *
 * \return retval_t: error code.
 */
retval_t set_propeller_duty(uint16_t duty);

#endif /* EXAMPLES_PROJECT_LIB_PROPELLER_INC_PROPELLER_UTILS_H_ */
