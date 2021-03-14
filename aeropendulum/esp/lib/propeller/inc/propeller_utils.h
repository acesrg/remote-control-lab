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

retval_t initialize_propeller(void);
retval_t deinitialize_propeller(void);
retval_t set_propeller_duty(uint16_t duty);

#endif /* EXAMPLES_PROJECT_LIB_PROPELLER_INC_PROPELLER_UTILS_H_ */
