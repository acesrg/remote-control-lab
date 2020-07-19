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
#include <espressif/esp_common.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ssid_config.h>
#include <httpd/httpd.h>
#include <http_server.h>
#include <json_parser.h>
#include <callback_test.h>

TestRvType test_uri_parsing(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode) {
        log_info("parse test, inputs: %s", data);
        SimpleJSONType received_data[1];
        snprintf(received_data[0].name, TOKEN_NAME_MAX_LEN, "duty");
        quick_get_value((const char *) data, received_data);


        char composed_echo_json[JSON_ACTUATOR_MAX_LEN];
        size_t received_data_size = sizeof(received_data)/sizeof(*received_data);
        json_simple_compose(composed_echo_json, received_data, received_data_size);

        websocket_write(pcb, (uint8_t *) composed_echo_json, strlen(composed_echo_json), WS_TEXT_MODE);

        for (int i = 0; i < received_data_size; i++) {
            log_info("parsing result: (name: %s, value:%f)", received_data[i].name, received_data[i].value);
        }
        log_info("composition result: %s", composed_echo_json);
        return TEST_OK;
}
