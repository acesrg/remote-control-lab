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
/** \file send_telemetry.h */
#ifndef EXAMPLES_PROJECT_TASKS_INC_SEND_TELEMETRY_H_
#define EXAMPLES_PROJECT_TASKS_INC_SEND_TELEMETRY_H_

/*
 * Telemetry definitions
 * */
#define MAX_TELEMETRY_PERIOD_ms     60000   /**< \brief Arbitrary max telemetry period */
#define DEFAULT_TELEMETRY_PERIOD_ms 500     /**< \brief Comfortable telemetry period */
#define MIN_TELEMETRY_PERIOD_ms     1       /**< \brief Min telemetry period (sw limitation) */

/**
 * \brief Initializes encoder, then sends telemetry in a loop.
 *
 * \param *pvParameter: Basically, the TCP socket structure.
 */
void send_telemetry_task(void *pvParameter);

#endif /* EXAMPLES_PROJECT_TASKS_INC_SEND_TELEMETRY_H_ */
