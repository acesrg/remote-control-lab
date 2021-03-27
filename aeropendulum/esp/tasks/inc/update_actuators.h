/*
 * Copyright 2020 Marco Miretti.
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
/** \file update_actuators.h */
#ifndef EXAMPLES_PROJECT_TASKS_INC_UPDATE_ACTUATORS_H_
#define EXAMPLES_PROJECT_TASKS_INC_UPDATE_ACTUATORS_H_

/*
 * Telemetry definitions
 * */
#define MAX_ACTUATORS_PERIOD_ms             60000   /**< \brief Arbitrary max actuator refresh period */
#define DEFAULT_ACTUATORS_UPDATE_PERIOD_ms  50      /**< \brief Comfortable actuator refresh period */
#define MIN_ACTUATORS_PERIOD_ms             5       /**< \brief Min refresh period (hw limit) */


/**
 * \brief   Update the actuators when the value has changed.
 * \param   *pvParameter: Unused.
 */
void update_actuators_task(void *pvParameter);

#endif /* EXAMPLES_PROJECT_TASKS_INC_UPDATE_ACTUATORS_H_ */
