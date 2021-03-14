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
#include <FreeRTOS.h>
#include <semphr.h>

#include <json_parser.h>

#include <telemetry_callback.h>

extern simple_json_t actuator_db[1];

extern SemaphoreHandle_t xMutex_actuator_data;

CallbackRvType telemetry_callback_handler(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode) {
    data[data_len] = '\0';
    /*
     * then, once the response was written to the websocket start
     * processing the input data.
     *
     * since the websocket stream database is being used, and the
     * json shold have the form of {"duty": %f}. We pass directly
     * the structure (initialized with said form).
     *
     * after this call, actuator_db.value should be updated and
     * ready to use.
     * */
    if (xMutex_actuator_data != NULL) {
        /* See if we can obtain the actuator_db mutex */
        if (xSemaphoreTake(xMutex_actuator_data, (TickType_t) 100) == pdTRUE) {
            retval_t parse_rv = quick_get_value((const char *) data, actuator_db);

            xSemaphoreGive(xMutex_actuator_data);

            if (parse_rv != RV_OK) {
                return CALLBACK_PARSE_ERROR;
            }
        }
    }

    /* if we get to this poin everything went ok! */
    return CALLBACK_OK;
}
